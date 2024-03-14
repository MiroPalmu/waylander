#pragma once

/// @file Contains stuff related to Wayland protocol

#include <cstdint>

#include "linux_utils/file_descriptor.hpp"

namespace ger {
namespace wl {

/// Represents one connected client by wrapping the Wayland socket.
class connected_client {
    linux::fd_type wayland_fd_{};

  public:
    [[nodiscard]] connected_client();
};

} // namespace wl
} // namespace ger
