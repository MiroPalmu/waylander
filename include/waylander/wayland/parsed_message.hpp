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
/// Declares vocabulary type parsed_message.

#include <cstddef>
#include <span>

#include "waylander/wayland/protocol_primitives.hpp"

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
