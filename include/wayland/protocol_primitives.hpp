#pragma once

/// @file Contains primitives used in protocol.hpp implementation.

#include <bit>
#include <climits>
#include <concepts>
#include <cstdint>
#include <functional>
#include <string_view>
#include <type_traits>

#include "gnu_utils/fd_handle.hpp"

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

    /// Reserve new id from client, assign it to this object and return copy of it.
    ///
    /// To prevent inclusion of connected_client, it is forward declared.
    /// This means that its members can not be used, so this function
    /// has to be defined after connected_client is defined.
    ///
    /// So, to use this function one has to include connected_client as well.
    /// As that header will always include this one, there is no problem with order of includes.
    Wobject<WObj> reserve_id(class connected_client& client);
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

struct Wfd {
    gnu::fd_ref value;
};

struct Wmessage_size_t {
    using integral_type = std::uint16_t;
    integral_type value;
};

template<interface T>
struct Wopcode {
    using integral_type = std::uint16_t;
    integral_type value;
};

static_assert(std::endian::native == std::endian::little,
              "Only little edian platforms are supported atm.");

/// Type representing Wayland wire format message header.
///
/// Order of opcode and size is dependent on a system edianess,
/// as they are endcoded as 32-bit word and size is defined to be in the upper 16-bits
/// and the op code in lower 16-bits.
///
/// At the moment little edian platform is supported, which is represented in current order.
template<interface WObj>
struct message_header {
    Wobject<WObj> object_id;
    Wopcode<WObj> opcode;
    Wmessage_size_t size;

    static constexpr std::size_t opcode_offset = sizeof(object_id);
    static constexpr std::size_t size_offset   = opcode_offset + sizeof(opcode);

    /// Constructor to disable aggregate initialization.
    ///
    /// This is wanted in anticipation of big edian platform support.
    [[nodiscard]] constexpr explicit message_header(const Wobject<WObj> id,
                                                    const Wopcode<WObj> op,
                                                    const Wmessage_size_t size_a)
        : object_id{ id },
          opcode{ op },
          size{ size_a } {}
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
