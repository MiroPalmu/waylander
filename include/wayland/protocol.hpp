#pragma once

/// @file Contains stuff related to Wayland protocol

#include <cstdint>

#include "linux_utils/file_descriptor.hpp"

namespace ger {
namespace wl {

/// Represents the compositor by wrapping file descriptor connected to wayland socket.
class compositor {
    linux::fd_type wayland_fd_{};

  public:
    [[nodiscard]] compositor();
};

} // namespace wl
} // namespace ger
