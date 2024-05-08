#pragma once

/// @file Implements container of Wayland protocol messages.

#include <cstddef>
#include <ranges>
#include <vector>

#include "byte_vec.hpp"
#include "construct_allocator_adapter.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

class message_buffer {
    bool is_empty_ = true;

  public:
    constexpr message_buffer() {}

    /// True if buffer contains any messages.
    constexpr bool empty() { return is_empty_; }

    template<interface WObj, message_for_inteface<WObj> Message>
    constexpr void append(const Wobject<WObj> obj, const Message& msg) {
        is_empty_ = false;
        [](auto...) {}(obj, msg);
    }

    constexpr auto release_data() -> sstd::byte_vec {
        is_empty_ = true;
        return {};
    };
};

} // namespace wl
} // namespace ger
