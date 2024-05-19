#pragma once

/// @file Contains stuff related to Wayland protocol

#include <cstdint>
#include <vector>

#include "gnu_utils/local_stream_socket.hpp"
#include "linux_utils/file_descriptor.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

namespace ger {
namespace wl {

static constexpr auto global_display_object = Wobject<protocols::wl_display>{ 1 };

/// Represents one connected client by wrapping the Wayland socket.
class connected_client {
    linux::fd_type wayland_fd_{};
    Wuint::integral_type next_new_id_{ 2 };

    bool has_requests_{ false };

  public:
    [[nodiscard]] connected_client();

    /// Uses given \p socket as the compositor socket.
    [[nodiscard]] connected_client(gnu::local_stream_socket&& socket);

    template<typename T = generic_object>
    [[nodiscard]] auto reserve_object_id() -> Wobject<T> {
        return { next_new_id_++ };
    }

    template<interface WObj, message_for_inteface<WObj> request>
    void register_request(const Wobject<WObj> /* object */, const request& /* message */) {
        has_requests_ = true;
    }

    void flush_registered_requests();

    [[nodiscard]] constexpr bool has_registered_requests(this auto&& self) noexcept {
        return self.has_requests_;
    }
};

} // namespace wl
} // namespace ger
