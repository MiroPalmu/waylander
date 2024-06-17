#pragma once

/// @file Implements interpretation of raw bytes based on object id and opcode.

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <functional>
#include <memory>
#include <ranges>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "wayland/message_utils.hpp"
#include "wayland/parsed_message.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

/// Stores information to interpert raw bytes as Wayland messages.
class message_visitor {
    std::move_only_function<void()> default_overload_;
    bool has_overload_{ false };
    using key_t             = std::pair<Wobject<generic_object>, Wopcode<generic_object>>;
    using erased_overload_t = std::move_only_function<void(std::span<const std::byte>)>;

    struct key_t_hash {
        std::size_t operator()(const key_t& key) const noexcept {
            static_assert(sizeof(std::size_t) >= sizeof(key.first) + sizeof(key.second));

            std::array<std::byte, sizeof(std::size_t)> combined;
            const auto first_at_combined  = combined.begin();
            const auto second_at_combined = std::ranges::next(combined.begin(), sizeof(key.first));
            const auto leftover_at_combined =
                std::ranges::next(second_at_combined, sizeof(key.second));

            std::memcpy(first_at_combined, std::addressof(key.first), sizeof(key.first));
            std::memcpy(second_at_combined, std::addressof(key.second), sizeof(key.second));
            std::memset(leftover_at_combined,
                        0,
                        sizeof(std::size_t) - sizeof(key.first) - sizeof(key.second));

            static_assert(sizeof(combined) == sizeof(std::size_t));
            return std::hash<std::size_t>{}(std::bit_cast<std::size_t>(combined));
        }
    };

    std::unordered_map<key_t, erased_overload_t, key_t_hash> overloads_{};

  public:
    template<std::invocable F>
    [[nodiscard]] explicit message_visitor(F&& default_overload)
        : default_overload_{ std::forward<F>(default_overload) } {};

    template<typename Msg, interface W>
    void add_overload(const Wobject<W> obj_id, std::invocable<Msg> auto&& overload_arg) {
        const auto [_, insert_ok] = overloads_.insert(
            { key_t{ obj_id.value, Msg::opcode.value },
              [overload = std::forward<decltype(overload_arg)>(overload_arg)](
                  const std::span<const std::byte> bytes) mutable {
                  using msg_primitives_as_tuple = decltype(sstd::to_tuple(std::declval<Msg>()));
                  constexpr auto amount_of_msg_primitives =
                      std::tuple_size_v<msg_primitives_as_tuple>;

                  auto bytes_read{ 0uz };

                  const auto read_element_32aligned = [&]<typename T>(std::type_identity<T>) -> T {
                      if constexpr (std::same_as<Wstring, T>) {
                          // Read size:
                          Wstring::size_type str_size;
                          std::memcpy(&str_size,
                                      std::addressof(bytes[bytes_read]),
                                      sizeof(str_size));

                          // Handle null string because if not here then (*) below is broken.
                          if (str_size == 0) {
                              // null-string
                              return Wstring{};
                          }

                          const auto str_begin =
                              std::addressof(bytes[bytes_read + sizeof(Wstring::size_type)]);

                          // Increment bytes_read:
                          const auto unpadded_total_size = sizeof(str_size) + str_size;
                          const auto padded_total_size =
                              unpadded_total_size
                              + sstd::round_upto_multiple_of<4>(unpadded_total_size);
                          bytes_read += padded_total_size;

                          static_assert(std::alignment_of_v<char8_t> == 1,
                                        "Sanity check for obscure platforms.");

                          return Wstring{ std::u8string_view(
#ifdef __cpp_lib_start_lifetime_as // Techincally UB without this.
                              std::start_lifetime_as_array<char8_t>(str_begin, str_size),
#else
                              reinterpret_cast<char8_t const*>(str_begin),
#endif
                              str_size - 1) }; // (*) -1 for null delimiter
                      } else if constexpr (std::same_as<Warray, T>) {
                          // Read size:
                          Warray::size_type array_size;
                          std::memcpy(&array_size,
                                      std::addressof(bytes[bytes_read]),
                                      sizeof(array_size));

                          const auto array_begin =
                              std::addressof(bytes[bytes_read + sizeof(Warray::size_type)]);

                          // Increment bytes_read:
                          const auto unpadded_total_size = sizeof(array_size) + array_size;
                          const auto padded_total_size =
                              unpadded_total_size
                              + sstd::round_upto_multiple_of<4>(unpadded_total_size);
                          bytes_read += padded_total_size;

                          return Warray{ std::span(array_begin, array_size) };

                      } else if constexpr (static_message_argument<T>) {
                          constexpr auto pad          = sstd::round_upto_multiple_of<4>(sizeof(T));
                          constexpr auto element_size = sizeof(T) + pad;
                          T elem;
                          std::memcpy(std::addressof(elem),
                                      std::addressof(bytes[bytes_read]),
                                      sizeof(T));
                          bytes_read += element_size;
                          return elem;
                      }
                  };

                  [&]<std::size_t... I>(std::index_sequence<I...>) {
                      std::invoke(
                          overload,
                          Msg{ std::invoke(
                              read_element_32aligned,
                              std::type_identity<
                                  std::tuple_element_t<I, msg_primitives_as_tuple>>{})... });
                  }(std::make_index_sequence<amount_of_msg_primitives>());
              } });

        if (not insert_ok) {
            throw std::runtime_error{ "Overload for {Wobject<W>, Msg::opcode} already present!" };
        }
    }

    /// Invokes the overload corresponding to \p msg.object_id and \p msg.opcode with \p msg.arguments.
    ///
    /// Precondition: \p msg.arguments is a valid Wayland wire format message payload of
    /// message type corresponding to the resolved overload.
    void visit(const parsed_message& msg) {
        const auto overload_resolution = overloads_.find({ msg.object_id, msg.opcode });
        if (overload_resolution == overloads_.end()) {
            default_overload_();
        } else {
            std::invoke(overload_resolution->second, msg.arguments);
        }
    }

    /// Calls visit on messages in given ranges inorder.
    void visit(std::ranges::input_range auto&& msg_range) {
        // Offload any additional requirements of msg_range
        // to constraints of std::ranges::for_each.
        std::ranges::for_each(std::forward<decltype(msg_range)>(msg_range),
                              [this]<typename T>(T&& msg) { this->visit(std::forward<T>(msg)); });
    }
};

} // namespace wl
} // namespace ger
