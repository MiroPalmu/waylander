// Copyright (C) 2024 Miro Palmu.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file.  If not, see <https://www.gnu.org/licenses/>.

#include <config.h>

#include <generator>
#include <span>
#include <stdexcept>
#include <utility>

#include "byte_vec.hpp"
#include "gnu_utils/local_stream_socket.hpp"

#include "wayland/message_parser.hpp"
#include "wayland/message_visitor.hpp"
#include "wayland/connected_client.hpp"

namespace ger {
namespace wl {

[[nodiscard]] connected_client::connected_client(const std::filesystem::path& socket)
    : server_sock_{ gnu::open_local_stream_socket_connected_to(socket) } {}

[[nodiscard]] connected_client::connected_client(
    [[maybe_unused]] gnu::local_stream_socket&& server_sock)
    : server_sock_{ std::move(server_sock) } {};

void connected_client::flush_registered_requests() {
    auto data_to_write = request_buff_.release_data();
    auto fds_to_write  = request_buff_.release_fds();

    // Assume that there is more bytes to send than file descriptors.
    assert(data_to_write.size() > fds_to_write.size());

    // Then send each fd along one byte.
    auto bytes_written = 0uz;
    for (const auto& fd : fds_to_write) {
        auto msg =
            gnu::local_socket_msg<1, 1>{ std::span{ data_to_write }.subspan(bytes_written, 1),
                                         fd.value };
        bytes_written += server_sock_.send(msg);
    }

    // Finally send rest of the data.
    server_sock_.write(std::span{ data_to_write }.subspan(bytes_written));
};

void connected_client::recv_more_data() {
    // Let's say we are reading N bytes and U := unprocessed bytes in recv_buff_.
    //
    // As we assume that the data already in recv_buff_ begins a new message and
    // if M := U + N is not aligned to 32-bit words, then reading should stop
    // in the middle of a message. Wayland wire protocol guarantees that there
    // should be more data coming.

    // Magic number that could be made configurable.
    static constexpr auto recv_block_size = 1000uz;

    // Keeps track of the bytes that are actually from the server.
    auto unprocessed_bytes = recv_buff_.size();
read_again:
    const auto M          = unprocessed_bytes + recv_block_size;
    const auto is_aligned = (M % 4uz) == 0uz;
    const auto M_pad      = (is_aligned) ? 1uz : 0uz;

    recv_buff_.resize(M + M_pad);
    const auto begin_of_read = std::ranges::next(recv_buff_.begin(), unprocessed_bytes);
    const auto where_to_read = std::span<std::byte>(begin_of_read, recv_buff_.end());
    const auto bytes_read    = server_sock_.read_some(where_to_read);

    if (bytes_read == 0) {
        recv_buff_.resize(unprocessed_bytes);
        throw std::runtime_error{ "Encountered EOF from server socket!" };
    }

    unprocessed_bytes += bytes_read;

    if (bytes_read == where_to_read.size()) {
        // We read everything we asked for which means that:
        //
        //     A) it is likely that the socket still contains unread data
        //     B) the last byte read did not end 32-bit word.
        //
        // So it should be safe to read again.
        goto read_again;
    }

    recv_buff_.resize(unprocessed_bytes);
}

[[nodiscard]] auto connected_client::get_recd_bytes_forming_whole_messages()
    -> std::span<const std::byte> {
#ifdef __cpp_lib_is_implicit_lifetime
    static_assert(std::is_implicit_lifetime_v<wl::Wmessage_size_t>);
#endif

    auto bytes_forming_whole_messages = 0uz;

try_next_msg:
    const auto bytes_required_for_next_msg_size =
        bytes_forming_whole_messages + sizeof(message_header<generic_object>);

    if (recv_buff_.size() < bytes_required_for_next_msg_size) {
        return std::span(recv_buff_.data(), bytes_forming_whole_messages);
    }

    wl::Wmessage_size_t size_of_next_msg;
    const auto ptr_to_size = std::ranges::next(
        recv_buff_.begin(),
        bytes_forming_whole_messages + wl::message_header<wl::generic_object>::size_offset);
    std::memcpy(&size_of_next_msg, &*ptr_to_size, sizeof(wl::Wmessage_size_t));

    const auto bytes_potentially_forming_whole_messages =
        bytes_forming_whole_messages + size_of_next_msg.value;

    const auto potential_bytes_fit = recv_buff_.size() >= bytes_potentially_forming_whole_messages;

    if (potential_bytes_fit) {
        bytes_forming_whole_messages = bytes_potentially_forming_whole_messages;
        goto try_next_msg;
    }

    return std::span(recv_buff_.data(), bytes_forming_whole_messages);
}

[[nodiscard]] auto connected_client::recv_events() -> message_parser {
    recv_more_data();
    const auto bytes_to_parse = get_recd_bytes_forming_whole_messages();
    auto parser               = message_parser{ bytes_to_parse };

    recv_buff_.erase(recv_buff_.begin(),
                     std::ranges::next(recv_buff_.begin(), bytes_to_parse.size()));
    return parser;
}

void connected_client::recvis_closure::until(
    const Wobject<generic_object> until_obj_id,
    const Wopcode<generic_object> until_opcode,
    const std::move_only_function<void(std::span<const std::byte>) const> callback) {
try_again:
    const auto bytes_to_parse = parent_obj_ref_.get_recd_bytes_forming_whole_messages();
    auto parsed_messages      = parsed_message_generator(bytes_to_parse);

    auto total_num_parsed_messages   = 0uz;
    auto total_parsed_argument_bytes = 0uz;

    for (const auto& msg : parsed_messages) {
        ++total_num_parsed_messages;
        total_parsed_argument_bytes += msg.arguments.size();

        if (msg.object_id == until_obj_id and msg.opcode == until_opcode) {
            /// Found "until message".

            if (callback) { callback(msg.arguments); }

            const auto total_parsed_bytes =
                sizeof(message_header<generic_object>) * total_num_parsed_messages
                + total_parsed_argument_bytes;

            parent_obj_ref_.recv_buff_.erase(
                parent_obj_ref_.recv_buff_.begin(),
                std::ranges::next(parent_obj_ref_.recv_buff_.begin(), total_parsed_bytes));

            return;
        }

        const auto ov_res = mos_.overload_resolution(msg.object_id, msg.opcode);

        // Skip messages without overloads.
        if (ov_res.has_value()) { std::invoke(ov_res.value(), msg.arguments); }
    }

    // "Until message" was not found from already recevided whole messages.

    // Erease visited messages.
    parent_obj_ref_.recv_buff_.erase(
        parent_obj_ref_.recv_buff_.begin(),
        std::ranges::next(parent_obj_ref_.recv_buff_.begin(), bytes_to_parse.size()));

    parent_obj_ref_.recv_more_data();
    goto try_again;
}

auto connected_client::recv_and_visit_events(message_overload_set& mos)
    -> connected_client::recvis_closure {
    return recvis_closure(*this, mos);
}

} // namespace wl
} // namespace ger
