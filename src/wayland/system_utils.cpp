#include <config.h>

/// @file Implementations of utilities for inspecting the surrounding system.

#include <cstddef>
#include <filesystem>
#include <format>
#include <stdexcept>

namespace ger {
namespace wl {

auto wayland_socket_path() -> std::filesystem::path {
    const auto wayland_display_cstr = std::getenv("WAYLAND_DISPLAY");

    auto wayland_socket_name = std::filesystem::path{ "wayland-0" };

    if (wayland_display_cstr != nullptr) {
        const auto wayland_display = std::filesystem::path{ wayland_display_cstr };

        if (wayland_display.is_absolute()) {
            return wayland_display;
        } else if (wayland_display.is_relative()) {
            wayland_socket_name = wayland_display;
        } else {
            throw std::logic_error(std::format("Path '{}' is neither absolute nor relative!?",
                                               wayland_display.native()));
        }
    }

    const auto runtime_dir_cstr = std::getenv("XDG_RUNTIME_DIR");

    if (runtime_dir_cstr == nullptr) {
        throw std::runtime_error(
            "XDG_RUNTIME_DIR is required if WAYLAND_DISPLAY is not absolute path!");
    }

    return std::filesystem::path{ runtime_dir_cstr } / wayland_socket_name;
}

} // namespace wl
} // namespace ger
