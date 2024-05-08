#pragma once

/// @file Implements container of Wayland protocol messages.

#include <cstddef>
#include <generator>
#include <ranges>
#include <span>
#include <vector>

#include "byte_vec.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

/// Parsed Wayland message from raw bytes.
///
/// Note that by reading the raw bytes, there is not enough information to determine the type of the object.
struct parsed_message {
    wl::Wobject<wl::generic_object> object_id;
    wl::Wopcode<wl::generic_object> opcode;
};

class message_parser {
    sstd::byte_vec unparsed_messages_;

  public:
    [[nodiscard]] constexpr message_parser(const std::span<const std::byte> input)
        : unparsed_messages_(input.begin(), input.end()) {}

    [[nodiscard]] auto message_generator() -> std::generator<const parsed_message&>;
};

} // namespace wl
} // namespace ger
