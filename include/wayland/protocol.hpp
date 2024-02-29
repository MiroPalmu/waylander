#pragma once

/// @file Contains stuff related to Wayland protocol

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <optional>
#include <print>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>

#include <sys/socket.h>
#include <sys/un.h>

#include "bit_fiddling.hpp"

namespace ger {
namespace wl {

static constexpr std::uint32_t wayland_display_object_id                   = 1;
static constexpr std::uint16_t wayland_wl_registry_event_global            = 0;
static constexpr std::uint16_t wayland_shm_pool_event_format               = 0;
static constexpr std::uint16_t wayland_wl_buffer_event_release             = 0;
static constexpr std::uint16_t wayland_xdg_wm_base_event_ping              = 0;
static constexpr std::uint16_t wayland_xdg_toplevel_event_configure        = 0;
static constexpr std::uint16_t wayland_xdg_toplevel_event_close            = 1;
static constexpr std::uint16_t wayland_xdg_surface_event_configure         = 0;
static constexpr std::uint16_t wayland_wl_display_get_registry_opcode      = 1;
static constexpr std::uint16_t wayland_wl_registry_bind_opcode             = 0;
static constexpr std::uint16_t wayland_wl_compositor_create_surface_opcode = 0;
static constexpr std::uint16_t wayland_xdg_wm_base_pong_opcode             = 3;
static constexpr std::uint16_t wayland_xdg_surface_ack_configure_opcode    = 4;
static constexpr std::uint16_t wayland_wl_shm_create_pool_opcode           = 0;
static constexpr std::uint16_t wayland_xdg_wm_base_get_xdg_surface_opcode  = 2;
static constexpr std::uint16_t wayland_wl_shm_pool_create_buffer_opcode    = 0;
static constexpr std::uint16_t wayland_wl_surface_attach_opcode            = 1;
static constexpr std::uint16_t wayland_xdg_surface_get_toplevel_opcode     = 1;
static constexpr std::uint16_t wayland_wl_surface_commit_opcode            = 6;
static constexpr std::uint16_t wayland_wl_display_error_event              = 0;
static constexpr std::uint32_t wayland_format_xrgb8888                     = 1;
static constexpr std::uint32_t wayland_header_size                         = 8;
static constexpr std::uint32_t color_channels                              = 4;

inline auto display_connect() {
    namespace fs       = std::filesystem;
    const auto get_env = [](const std::string& name) -> std::optional<std::string> {
        const auto env = std::getenv(name.data());
        return env ? std::string{ env } : std::optional<std::string>{};
    };

    const auto xdg_runtime_dir = get_env("XDG_RUNTIME_DIR");
    const auto wayland_display = get_env("WAYLAND_DISPLAY");

    const auto xdg_rt_dir = get_env("XDG_RUNTIME_DIR");
    if (not xdg_rt_dir) throw std::runtime_error{ "XDG_RUNTIME_DIR is not set" };

    const auto socket_path =
        (fs::path{ xdg_rt_dir.value() } / get_env("WAYLAND_DISPLAY").value_or("wayland-0"))
            .string();

    if (socket_path.size() >= 107)
        std::runtime_error{ "sockaddr_un.sun_path max path length is 108 chars" };

    auto socket_addres = [&] {
        auto addres = sockaddr_un{ .sun_family = AF_UNIX };
        std::memcpy(addres.sun_path, socket_path.c_str(), socket_path.size());
        return addres;
    }();

    const auto fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) throw std::runtime_error{ "could not open socket" };

    if (connect(fd, reinterpret_cast<sockaddr*>(&socket_addres), sizeof(socket_addres)) == -1)
        throw std::runtime_error{ "could not connect to the socket to filedescriptor" };

    return fd;
}

inline auto display_get_registery(const int fd) {
    static std::uint32_t wayland_current_id = 1;
    constexpr auto message =
        bit::byte_buff(wayland_display_object_id,
                       wayland_wl_display_get_registry_opcode,
                       static_cast<std::uint16_t>(wayland_header_size + sizeof(wayland_current_id)),
                       wayland_current_id++);

    constexpr auto span_of_message = message.bytes();
    constexpr auto send_bytes =
        send(fd, span_of_message.data(), span_of_message.size(), MSG_DONTWAIT);
    if (std::ranges::ssize(span_of_message) != send_bytes) {
        throw std::runtime_error{ std::format("could not write to file descriptor: {}", fd) };
    }

    std::print("-> wl_display@{}.get_registery: wl_registery={}",
               wayland_display_object_id,
               wayland_current_id);
    return wayland_current_id;
}

} // namespace wl
} // namespace ger
