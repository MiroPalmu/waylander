#pragma once

/// @file Implements storage of overloads/listeners/callbacks/... for messages.

#include <concepts>
#include <cstddef>
#include <functional>
#include <optional>
#include <span>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "wayland/message_utils.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

/// A tool to interpert parsed_message as a message type and pass it to correct overload (callback).
///
/// When interperting raw bytes from parsed_message arguments as a Wayland primitives,
/// the spesific message type has to be inferred based on the object id and opcode.
/// To do this the corresponding interface for the object id has to be stored.
///
/// This type tries to do that with increased type safety.
///
/// Note that the overloads can hold a state, so to make the implementation simpler,
/// a const message_overload_set is not supported.
class message_overload_set {
    using key_t = std::pair<Wobject<generic_object>, Wopcode<generic_object>>;

    struct key_t_hash {
        std::size_t operator()(const key_t& key) const noexcept;
    };

    /// The overloads have different call signatures, so they have to be type erased.
    using erased_overload_t = std::move_only_function<void(std::span<const std::byte>)>;

    std::unordered_map<key_t, erased_overload_t, key_t_hash> overloads_{};

  public:
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

    /// Finds overload corresponding to {object id, opcode}-pair or returns empty optional.
    auto overload_resolution(const Wobject<generic_object> obj_id,
                             const Wopcode<generic_object> opcode)
        -> std::optional<std::reference_wrapper<erased_overload_t>>;
};

} // namespace wl
} // namespace ger
