#pragma once

/// @file Contains primitives used in protocol.hpp implementation.

#include <bit>
#include <climits>
#include <concepts>
#include <cstdint>
#include <functional>
#include <string_view>

#include "bit_fiddling.hpp"
#include "linux_utils/file_descriptor.hpp"
#include "sstd.hpp"

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

struct Wint : sstd::integralifyer<std::int32_t> {};
struct Wuint : sstd::integralifyer<std::uint32_t> {};
struct Wfixed {
    bool is_negative : 1;
    unsigned mantissa : 23;
    unsigned exponent : 8;
};

/// Forward decleration for converting operator in Wobject.
template<interface T>
struct Wnew_id;

template<interface WObj = generic_object>
struct Wobject : sstd::integralifyer<sstd::underlying_integral_t<Wuint>> {
    template<interface T>
        requires std::same_as<T, WObj> or std::same_as<T, generic_object>
    [[nodiscard]] constexpr operator Wnew_id<T>(this auto&& self) {
        return { self.value };
    }
};

template<interface WObj = generic_object>
struct Wnew_id : sstd::integralifyer<sstd::underlying_integral_t<Wobject<WObj>>> {};

struct Wstring : std::u8string_view {
    using size_type = std::uint32_t;
};
struct Warray : std::span<const std::byte> {
    using size_type = std::uint32_t;
};
struct Wfd {};

struct Wmessage_size_t : sstd::integralifyer<std::uint16_t> {};

template<interface T>
struct Wopcode : sstd::integralifyer<std::uint16_t> {};

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

/// Deduction guide for Wnew_id to make Wnew_id{ 4u } work.
///
/// Wobject{ 4u } will work while Wnew_id{ 4u } does not.
/// What is supposed to happen is that compiler will implicitly generate
/// class template deduction guides, which should contain aggregate deduction candidates.
///
/// Below is adhoc aggregate diagrams in which we define I.t := underlying_integral_t<I>:
///
///     Wobject<X>                      Wnew_id<X>
///     |                               |
///     { { I } }                       { { J } }
///       |                               |
///       integralifyer<Wuint.t =: I>     integralifyer<Wobject<X>.t = Wuint.t =: J>
///
/// Becuase of brace elision aggregate element of Wobject<X> is the sole I,
/// following aggregate deduction candidate is produced:
///
///     template <typename T>
///     Wobject<T> F(T);
///
/// which will be found by overload resolution in CDAT.
///
/// However for Wnew_id the aggregate element is { J }, as in CDAT brace elision is not considered
/// for any aggregate element that has a dependent non-array type.
/// In this case { J } is dependent type as it depends on the template argument X.
/// With Wobject I is not dependent type so brace elision is applied.
///
/// So the produced deduction candidate is:
///
///     template <typename T>
///     Wnew_id<T> F(integralifyer<T>);
///
/// which will not be found by overload resolution in CDAT and CDAT will fail.
///
/// This can be fixed by providing the deduction guide:
template<typename T>
Wnew_id(T) -> Wnew_id<generic_object>;

template<typename M, typename Interface>
concept message_for_inteface = interface<Interface> and requires {
    { M::opcode } -> std::same_as<const Wopcode<Interface>&>;
};

} // namespace wl
} // namespace ger
