#pragma once

#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstring>
#include <span>
#include <type_traits>
#include <variant>
#include <vector>

namespace ger {
namespace sstd {

template<std::size_t N, typename... I>
concept total_memory_usage_is = (N == (0 + ... + sizeof(I)));

/// Stores bytes in std::array with native endianness
template<std::size_t N>
class byte_array {
    std::array<std::byte, N> data_{};

  public:
    template<std::unsigned_integral... I>
        requires total_memory_usage_is<N, I...>
    [[nodiscard]] constexpr byte_array(const I... message) {
        auto pos = std::size_t{ 0 };

        auto write_bytes_to_pos = [&]<std::size_t... Bytes>(const std::unsigned_integral auto x,
                                                            std::index_sequence<Bytes...>) {
            ((data_[pos + Bytes] =
                  std::bit_cast<std::byte>(static_cast<std::byte>(x >> 8 * Bytes))),
             ...);
        };

        [[maybe_unused]] auto write_and_increment_pos = [&](const std::unsigned_integral auto x) {
            write_bytes_to_pos(x, std::make_index_sequence<sizeof(decltype(x))>{});

            pos += sizeof(decltype(x));
        };

        (write_and_increment_pos(message), ...);
    }

    template<typename... T>
        requires total_memory_usage_is<N, T...> and (std::is_trivially_copyable_v<T> and ...)
                 and (not(std::unsigned_integral<T> and ...))
    [[nodiscard]] byte_array(const T&... message) {
        auto pos = std::size_t{ 0 };

        [[maybe_unused]] auto write_and_increment_pos = [&]<typename U>(const U& x) {
            std::memcpy(std::addressof(data_[pos]), std::addressof(x), sizeof(U));
            pos += sizeof(decltype(x));
        };

        (write_and_increment_pos(message), ...);
    }

    [[nodiscard]] constexpr auto bytes(this auto&& self) -> std::span<std::byte const, N> {
        return self.data_;
    }

    [[nodiscard]] friend constexpr bool operator==(const byte_array& lhs,
                                                   const byte_array& rhs) = default;
};

template<std::unsigned_integral... I>
byte_array(I...) -> byte_array<(0 + ... + sizeof(I))>;

} // namespace sstd
} // namespace ger
