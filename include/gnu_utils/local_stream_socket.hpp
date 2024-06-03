#pragma once

#include <filesystem>
#include <tuple>

#include "gnu_utils/fd_handle.hpp"
#include "sstd.hpp"

namespace ger {
namespace gnu {

using local_stream_socket_fd = sstd::unique_handle<fd_handle, close_fd_handle>;

/// Connected stream style socket in local namespace.
class local_stream_socket : private local_stream_socket_fd {
    using local_stream_socket_fd::local_stream_socket_fd;

    friend auto open_local_stream_socket_pair()
        -> std::pair<local_stream_socket, local_stream_socket>;

    friend auto open_local_stream_socket_connected_to(const std::filesystem::path&)
        -> local_stream_socket;

  public:
    using local_stream_socket_fd::read;
    using local_stream_socket_fd::read_some;
    using local_stream_socket_fd::write;
};

[[nodiscard]] auto open_local_stream_socket_pair()
    -> std::pair<local_stream_socket, local_stream_socket>;

[[nodiscard]] auto open_local_stream_socket_connected_to(const std::filesystem::path&)
    -> local_stream_socket;

} // namespace gnu
} // namespace ger
