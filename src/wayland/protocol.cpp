/// @file Implements wl::compsitor

#include <config.h>

#include <array>
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
#include "linux_utils/file_descriptor.hpp"
#include "sstd.hpp"

#include "wayland/protocol.hpp"

namespace ger {
namespace wl {

/// Opens a file descriptor connected to wayland socket
auto connect_to_wayland_socket() -> linux::fd_type {
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

    if (socket_path.size() >= 107) {
        throw std::runtime_error{ std::format(
            "Path \"{}\"\nis logner than max path length (108 chars) for sockaddr_un::sun_path",
            socket_path) };
    }

    auto socket_addres = [&] {
        auto addres = sockaddr_un{ .sun_family = AF_UNIX, .sun_path = {} };
        std::memcpy(addres.sun_path, socket_path.c_str(), socket_path.size());
        return addres;
    }();

    auto fd = sstd::legacy_construct<linux::fd_type>(socket, AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) throw std::runtime_error{ "could not open socket" };

    if (connect(fd, reinterpret_cast<sockaddr*>(&socket_addres), sizeof(socket_addres)) == -1)
        throw std::runtime_error{ "could not connect to the socket to filedescriptor" };

    return fd;
}

[[nodiscard]] connected_client::connected_client(
    [[maybe_unused]] gnu::local_stream_socket&& server_sock)
    : server_sock_{ std::move(server_sock) } {};

void connected_client::flush_registered_requests() {
    const auto data_to_write = msg_buff_.release_data();
    server_sock_.write(data_to_write);
};
} // namespace wl
} // namespace ger
