#pragma once

/// @file Utilities for handling Wayland messages.

#include <concepts>
#include <type_traits>

#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

template<typename>
struct is_any_Wobject : std::false_type {};
template<interface WObj>
struct is_any_Wobject<Wobject<WObj>> : std::true_type {};

template<typename>
struct is_any_Wnew_id : std::false_type {};
template<interface WObj>
struct is_any_Wnew_id<Wnew_id<WObj>> : std::true_type {};

template<typename A>
concept static_message_argument =
    std::same_as<A, Wint> or std::same_as<A, Wuint> or std::same_as<A, Wfixed>
    or is_any_Wobject<A>::value or is_any_Wnew_id<A>::value;

template<typename A>
concept dynamic_message_argument =
    std::same_as<A, Wstring> or std::same_as<A, Warray> or std::same_as<A, Wfd>;

} // namespace wl
} // namespace ger
