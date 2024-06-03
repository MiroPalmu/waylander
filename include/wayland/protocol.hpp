#pragma once

/// @file Contains stuff related to Wayland protocol

#include <cstdint>
#include <filesystem>
#include <vector>

#include "gnu_utils/local_stream_socket.hpp"
#include "wayland/message_buffer.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"
#include "wayland/system_utils.hpp"

namespace ger {
namespace wl {

static constexpr auto global_display_object = Wobject<protocols::wl_display>{ 1 };

/// Represents one connected client by wrapping the Wayland socket.
class connected_client {
    gnu::local_stream_socket server_sock_;
    Wuint::integral_type next_new_id_{ 2 };
    message_buffer msg_buff_{};

  public:
    /// Connected to socket at \p socket.
    [[nodiscard]] connected_client(const std::filesystem::path& socket = wayland_socket_path());

    /// Uses given \p socket as the compositor socket.
    [[nodiscard]] connected_client(gnu::local_stream_socket&& socket);

    template<typename T = generic_object>
    [[nodiscard]] auto reserve_object_id() -> Wobject<T> {
        return { next_new_id_++ };
    }

    template<interface WObj, message_for_inteface<WObj> request>
    void register_request(const Wobject<WObj> obj, const request& msg) {
        msg_buff_.append(obj, msg);
    }

    void flush_registered_requests();

    [[nodiscard]] constexpr bool has_registered_requests(this auto&& self) noexcept {
        return not self.msg_buff_.empty();
    }
};

} // namespace wl
} // namespace ger
