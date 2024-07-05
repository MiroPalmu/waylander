#pragma once

/// @file
/// Implements interpretation of raw bytes based on object id and opcode.

#include <algorithm>
#include <concepts>
#include <functional>
#include <utility>

#include "wayland/message_overload_set.hpp"
#include "wayland/parsed_message.hpp"

namespace ger {
namespace wl {

/// Invokes overload from \p mos corresponding to \p msg if present or invokes \p default_overload.
///
/// Precondition: \p msg.arguments is a valid Wayland wire format message payload of
/// message type corresponding to the resolved overload.
template<typename F>
    requires std::invocable<F> or std::invocable<F, const parsed_message&>
void message_visit(F&& default_overload, message_overload_set& mos, const parsed_message& msg) {
    const auto overload_resolution = mos.overload_resolution(msg.object_id, msg.opcode);
    if (overload_resolution.has_value()) {
        std::invoke(overload_resolution.value(), msg.arguments);
    } else {
        if constexpr (std::invocable<F>) {
            std::invoke(default_overload);
        } else {
            std::invoke(default_overload, msg);
        }
    }
}

/// Like message_visit(..., const parsed_message&) but for range of them.
template<typename F>
    requires std::invocable<F> or std::invocable<F, const parsed_message&>
void message_visit(F&& default_overload,
                   message_overload_set& mos,
                   std::ranges::input_range auto&& msg_range) {
    // Offload any additional requirements of msg_range to constraints of std::ranges::for_each.
    std::ranges::for_each(
        std::forward<decltype(msg_range)>(msg_range),
        [&](const parsed_message& msg) { message_visit(default_overload, mos, msg); });
}

} // namespace wl
} // namespace ger
