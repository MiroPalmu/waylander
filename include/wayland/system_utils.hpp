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
/// Utilities for inspecting the surrounding system.

#include <filesystem>

namespace ger {
namespace wl {

/// Determines path of the Wayland socket, but does not check if it actually exits.
///
/// Following describes the determination logic (followd step by step):
///
/// - If $WAYLAND_DISPLAY is absolute path, return $WAYLAND_DISPLAY.
/// - If $XDG_RUNTIME_DIR is not present throw std::runtime_error.
/// - If $WAYLAND_DISPLAY is relative path, return $XDG_RUNTIME_DIR/$WAYLAND_DISPLAY.
/// - If $WAYLAND_DISPLAY is not present, return $XDG_RUNTIME_DIR/wayland-0
///
auto wayland_socket_path() -> std::filesystem::path;

} // namespace wl
} // namespace ger
