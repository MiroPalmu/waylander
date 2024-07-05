#pragma once

/// @file Implements Wayland client side communication.

#include <cstdint>
#include <filesystem>
#include <functional>
#include <utility>

#include "gnu_utils/local_stream_socket.hpp"
#include "wayland/message_buffer.hpp"
#include "wayland/message_intrperter.hpp"
#include "wayland/message_overload_set.hpp"
#include "wayland/message_parser.hpp"
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
    message_buffer request_buff_{};
    /// Allways assumed that the data never begins at middle of message, only at a beginning.
    sstd::byte_vec recv_buff_{};

    /// Represents prepeared work to receive and visit messages.
    class recvis_closure {
        friend connected_client;
        connected_client& parent_obj_ref_;
        message_overload_set& mos_;

        [[nodiscard]] recvis_closure(connected_client& parent_obj_ref, message_overload_set& mos)
            : parent_obj_ref_{ parent_obj_ref },
              mos_{ mos } {}

        recvis_closure(const recvis_closure&)            = delete;
        recvis_closure(recvis_closure&&)                 = delete;
        recvis_closure& operator=(const recvis_closure&) = delete;
        recvis_closure& operator=(recvis_closure&&)      = delete;

        /// Receive and visit events until given (object id, opcode)-pair.
        ///
        /// Invokes the given function with the payload of the "until" message.
        void until(const Wobject<generic_object>,
                   const Wopcode<generic_object>,
                   const std::move_only_function<void(std::span<const std::byte>) const>);

      public:
        /// Receive and visit events until \p obj_id receives message \p Msg.
        template<typename Msg, interface W>
        void until(const Wobject<W> obj_id) && {
            until({ obj_id.value }, { Msg::opcode.value }, {});
        }

        /// Receive and visit events until \p obj_id receives message \p Msg.
        ///
        /// Invokes the given function with the payload of the "until" message.
        template<typename Msg, interface W>
        void until(const Wobject<W> obj_id, std::invocable<Msg> auto&& callback_arg) && {
            until({ obj_id.value },
                  { Msg::opcode.value },
                  [callback = std::forward<decltype(callback_arg)>(callback_arg)](
                      const std::span<const std::byte> payload) {
                      std::invoke(callback, interpert_message_payload<Msg>(payload));
                  });
        }
    };

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
        request_buff_.append(obj, msg);
    }

    void flush_registered_requests();

    [[nodiscard]] constexpr bool has_registered_requests(this auto&& self) noexcept {
        return not self.request_buff_.empty();
    }

    /// Receive non-zero amount of data to recv_buff_.
    void recv_more_data();

    /// Inspect recv_buff_ and get bytes from beginning that are checked to form whole messages.
    [[nodiscard]] auto get_recd_bytes_forming_whole_messages() -> std::span<const std::byte>;

    /// Read non-zero amount of bytes and return parser with all whole messages received.
    [[nodiscard]] auto recv_events() -> message_parser;

    auto recv_and_visit_events(message_overload_set&) -> recvis_closure;
};

/// See decleration.
template<interface T>
Wobject<T> Wobject<T>::reserve_id(connected_client& client) {
    return *this = client.reserve_object_id<T>();
}

} // namespace wl
} // namespace ger
