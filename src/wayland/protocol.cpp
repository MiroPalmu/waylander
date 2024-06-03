/// @file Implements wl::compsitor

#include <config.h>

#include <utility>

#include "gnu_utils/local_stream_socket.hpp"
#include "sstd.hpp"

#include "wayland/protocol.hpp"

namespace ger {
namespace wl {

[[nodiscard]] connected_client::connected_client(const std::filesystem::path& socket)
    : server_sock_{ gnu::open_local_stream_socket_connected_to(socket) } {}

[[nodiscard]] connected_client::connected_client(
    [[maybe_unused]] gnu::local_stream_socket&& server_sock)
    : server_sock_{ std::move(server_sock) } {};

void connected_client::flush_registered_requests() {
    const auto data_to_write = msg_buff_.release_data();
    server_sock_.write(data_to_write);
};
} // namespace wl
} // namespace ger
