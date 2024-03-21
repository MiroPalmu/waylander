#pragma once

/// @file Contains stuff related to Wayland protocol

#include <cstdint>

#include "linux_utils/file_descriptor.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

/// Represents one connected client by wrapping the Wayland socket.
class connected_client {
    linux::fd_type wayland_fd_{};
    Wuint::integral_type next_new_id_{ 2 };

  public:
    [[nodiscard]] connected_client();

    template<typename T = generic_object>
    [[nodiscard]] auto reserve_object_id() -> Wobject<T> {
        return { next_new_id_++ };
    }
};

} // namespace wl
} // namespace ger
