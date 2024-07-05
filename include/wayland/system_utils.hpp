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
