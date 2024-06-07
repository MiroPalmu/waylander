#pragma once

/// @file Contains primitives used in protocol.hpp implementation.

#include <bit>
#include <climits>
#include <concepts>
#include <cstdint>
#include <functional>
#include <string_view>
#include <type_traits>

#include "bit_fiddling.hpp"
#include "linux_utils/file_descriptor.hpp"

namespace ger {
namespace wl {

template<typename T>
concept interface = requires {
    typename T::request;
    typename T::event;
};

struct generic_object {
    struct request {};
    struct event {};
};

struct Wint {
    using integral_type = std::int32_t;
    integral_type value;
};
struct Wuint {
    using integral_type = std::uint32_t;
    integral_type value;
};
struct Wfixed {
    bool is_negative : 1;
    unsigned mantissa : 23;
    unsigned exponent : 8;
};

/// Forward decleration for converting operator in Wobject.
template<interface T>
struct Wnew_id;

template<interface WObj = generic_object>
struct Wobject {
    using integral_type = Wuint::integral_type;
    integral_type value;

    template<interface T>
        requires std::same_as<T, WObj> or std::same_as<T, generic_object>
    [[nodiscard]] constexpr operator Wnew_id<T>(this auto&& self) {
        return { self.value };
    }
};

template<interface WObj = generic_object>
struct Wnew_id {
    using integral_type = Wobject<WObj>::integral_type;
    integral_type value;
};

struct Wstring : std::u8string_view {
    /// The type used to store length of the string in Wire format.
    using size_type = std::uint32_t;
};
struct Warray : std::span<const std::byte> {
    /// The type used to store length of the string in Wire format.
    using size_type = std::uint32_t;
};
struct Wfd {};

struct Wmessage_size_t {
    using integral_type = std::uint16_t;
    integral_type value;
};

template<interface T>
struct Wopcode {
    using integral_type = std::uint16_t;
    integral_type value;
};

template<interface WObj>
struct message_header {
    Wobject<WObj> object_id;
    Wmessage_size_t size;
    Wopcode<WObj> opcode;
};

// Sanity check primitive sizes.
static_assert(CHAR_BIT == 8, "Guilander assumes std::byte to be 8 bits.");
static_assert(sizeof(Wint) == 4);
static_assert(sizeof(Wuint) == 4);
static_assert(sizeof(Wfixed) == 4);
static_assert(sizeof(Wobject<generic_object>) == 4);
static_assert(sizeof(Wnew_id<generic_object>) == 4);
static_assert(sizeof(message_header<generic_object>) == 8);

template<typename M, typename Interface>
concept message_for_inteface = interface<Interface> and requires {
    { M::opcode } -> std::same_as<const Wopcode<Interface>&>;
};

namespace {
template<typename T>
concept has_integral_value = requires(T t) {
    { auto{ t.value } } -> std::integral;
};

template<typename T>
concept morally_integral = std::integral<T> or has_integral_value<T>;

template<typename T>
    requires has_integral_value<T>
[[nodiscard]] constexpr auto get_integral_value(T&& x) noexcept {
    return std::forward<T>(x).value;
}

template<typename T>
    requires std::integral<std::remove_cvref_t<T>>
[[nodiscard]] constexpr auto get_integral_value(const T x) noexcept {
    return x;
}
}; // namespace

/// Comparison operator that message arguments can be compared to integers.
template<typename Lhs, typename Rhs>
    requires morally_integral<std::remove_cvref_t<Lhs>>
             and morally_integral<std::remove_cvref_t<Rhs>>
constexpr bool operator==(Lhs&& lhs, Rhs&& rhs) {
    return std::cmp_equal(get_integral_value(std::forward<Lhs>(lhs)),
                          get_integral_value(std::forward<Rhs>(rhs)));
}

} // namespace wl
} // namespace ger
