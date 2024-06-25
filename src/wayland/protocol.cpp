/// @file Implements wl::compsitor

#include <config.h>

#include <generator>
#include <stdexcept>
#include <utility>

#include "byte_vec.hpp"
#include "gnu_utils/local_stream_socket.hpp"

#include "wayland/protocol.hpp"

namespace ger {
namespace wl {

[[nodiscard]] connected_client::connected_client(const std::filesystem::path& socket)
    : server_sock_{ gnu::open_local_stream_socket_connected_to(socket) } {}

[[nodiscard]] connected_client::connected_client(
    [[maybe_unused]] gnu::local_stream_socket&& server_sock)
    : server_sock_{ std::move(server_sock) } {};

void connected_client::flush_registered_requests() {
    const auto data_to_write = request_buff_.release_data();
    server_sock_.write(data_to_write);
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

} // namespace wl
} // namespace ger
