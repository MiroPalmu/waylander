#pragma once

/// @file Implements parsing Wayland protocol messages from raw bytes.

#include <cstddef>
#include <generator>
#include <ranges>
#include <span>
#include <vector>

#include "byte_vec.hpp"
#include "wayland/parsed_message.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

class message_parser {
    sstd::byte_vec unparsed_messages_;

  public:
    [[nodiscard]] constexpr message_parser(const std::span<const std::byte> input)
        : unparsed_messages_(input.begin(), input.end()) {}

    [[nodiscard]] auto message_generator() -> std::generator<const parsed_message&>;
};

} // namespace wl
} // namespace ger
