#pragma once

/// @file
/// Declares vocabulary type parsed_message.

#include <cstddef>
#include <span>

#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

/// Parsed Wayland message from raw bytes.
///
/// Note that by reading the raw bytes, there is not enough information to determine the type of the object.
struct parsed_message {
    wl::Wobject<wl::generic_object> object_id;
    wl::Wopcode<wl::generic_object> opcode;
    std::span<const std::byte> arguments;
};

} // namespace wl
} // namespace ger
