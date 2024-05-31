#pragma once

/// @file Utilities for handling Wayland messages.

#include <concepts>
#include <type_traits>

#include "sstd.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

namespace {
template<typename>
struct is_any_Wobject : std::false_type {};
template<interface WObj>
struct is_any_Wobject<Wobject<WObj>> : std::true_type {};

template<typename>
struct is_any_Wnew_id : std::false_type {};
template<interface WObj>
struct is_any_Wnew_id<Wnew_id<WObj>> : std::true_type {};

template<typename A>
concept enum_message_argument =
    std::is_enum_v<A>
    and (std::same_as<sstd::underlying_integral_t<Wint>, std::underlying_type_t<A>>
         or std::same_as<sstd::underlying_integral_t<Wuint>, std::underlying_type_t<A>>);
} // namespace

template<typename A>
concept static_message_argument =
    std::same_as<A, Wint> or std::same_as<A, Wuint> or std::same_as<A, Wfixed>
    or is_any_Wobject<A>::value or is_any_Wnew_id<A>::value or enum_message_argument<A>;

template<typename A>
concept dynamic_message_argument =
    std::same_as<A, Wstring> or std::same_as<A, Warray> or std::same_as<A, Wfd>;

} // namespace wl
} // namespace ger
