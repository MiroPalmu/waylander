// Copyright (C) 2024 Miro Palmu.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

/// @file
/// Implements parsing Wayland protocol messages from raw bytes.

#include <cstddef>
#include <generator>
#include <ranges>
#include <span>
#include <vector>

#include "waylander/byte_vec.hpp"
#include "waylander/wayland/parsed_message.hpp"
#include "waylander/wayland/protocol_primitives.hpp"

namespace waylander {
namespace wl {

/// Parse messages from \p buff one at a time.
///
/// One at a time means that the next unparsed message is parsed when begin is
/// invoked from the retruned generator or when it is advaced.
///
/// Assumes that bytes int \p buff will form a correct Wayland wire format messages.
/// Throws if a partial message is detected.
///
/// Data pointed by \p buff is assumed to stay unchanched and within its liftime
/// throughout the lifetime of gnerator the coroutine state object.
[[nodiscard]] auto parsed_message_generator(const std::span<const std::byte> buff)
    -> std::generator<const parsed_message&>;

class message_parser {
    sstd::byte_vec unparsed_messages_;

  public:
    [[nodiscard]] constexpr message_parser(const std::span<const std::byte> input)
        : unparsed_messages_(input.begin(), input.end()) {}

    /// Returned generator can not outlive message_parser pointed by this pointer.
    [[nodiscard]] auto message_generator() -> std::generator<const parsed_message&>;
};

} // namespace wl
} // namespace waylander
