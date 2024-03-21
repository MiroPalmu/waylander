#pragma once

/// @file Contains primitives used in protocol.hpp implementation.

#include <concepts>
#include <cstdint>
#include <functional>

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

struct alignas(4) dynamic_argument {
    std::uint32_t size;
};

struct alignas(4) Wint : sstd::integralifyer<std::int32_t> {};
struct alignas(4) Wuint : sstd::integralifyer<std::uint32_t> {};
struct alignas(4) Wfixed {
    bool is_negative : 1;
    unsigned mantissa : 23;
    unsigned exponent : 8;
};
struct alignas(4) Wstring : dynamic_argument {};

/// Forward decleration for converting operator in Wobject.
template<interface T>
struct Wnew_id;

template<interface WObj = generic_object>
struct alignas(4) Wobject : sstd::integralifyer<sstd::underlying_integral_t<Wuint>> {
    template<interface T>
        requires std::same_as<T, WObj> or std::same_as<T, generic_object>
    [[nodiscard]] constexpr operator Wnew_id<T>(this auto&& self) {
        return { self.value };
    }
};

template<interface WObj = generic_object>
struct alignas(4) Wnew_id : sstd::integralifyer<sstd::underlying_integral_t<Wobject<WObj>>> {};

struct alignas(4) Warray : dynamic_argument {};
using Wfd = std::reference_wrapper<linux::fd_type>;

struct alignas(2) Wmessage_size_t : sstd::integralifyer<std::uint16_t> {};

template<interface T>
struct alignas(2) Wopcode : sstd::integralifyer<std::uint16_t> {};

template<interface WObj>
struct alignas(4) message_header {
    Wobject<WObj> object_id;
    Wmessage_size_t size;
    Wopcode<WObj> opcode;
};

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

} // namespace wl
} // namespace ger
