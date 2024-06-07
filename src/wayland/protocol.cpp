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

[[nodiscard]] auto connected_client::recv_events() -> message_parser {
    const auto recv_more_data = [&] {
        // Magic number that could be made configurable.
        static constexpr auto recv_block_size = 1000uz;

    read_again:
        // Assume that if recv_buff_ is not empty,
        // then the data in it begins a new message header.
        const auto unprocessed_bytes = recv_buff_.size();

        // Let's say we are reading N bytes and U := unprocessed_bytes.
        //
        // As we assume that the data already in recv_buff_ begins a new message,
        // if M := U + N is not aligned to 32-bit words, then the Wayland Wire Format
        // gurantees that, after reading N bytes, there is still unread data
        // in the socket and reading can be done again without blocking.
        //
        // This is because of every message argument must be alighend to 32-bits.
        const auto M          = unprocessed_bytes + recv_block_size;
        const auto is_aligned = (M % 4uz) == 0uz;
        const auto M_pad      = (is_aligned) ? 1uz : 0uz;

        recv_buff_.resize(M + M_pad);
        const auto begin_of_read = std::ranges::next(recv_buff_.begin(), unprocessed_bytes);
        const auto where_to_read = std::span<std::byte>(begin_of_read, recv_buff_.end());
        const auto bytes_read    = server_sock_.read_some(where_to_read);

        if (bytes_read == where_to_read.size()) {
            // As discussed above, we can now read again without blocking.
            goto read_again;
        }

        if (bytes_read == 0) {
            throw std::runtime_error{ "Encountered EOF from server socket!" };
        } else {
            recv_buff_.resize(unprocessed_bytes + bytes_read);
        }
    };

    const auto get_bytes_confirmed_to_form_whole_messages = [&]() -> std::size_t {
#ifdef __cpp_lib_is_implicit_lifetime
        static_assert(std::is_implicit_lifetime_v<wl::Wmessage_size_t>);
#endif

        auto bytes_forming_whole_messages = 0uz;

    try_next_msg:
        const auto bytes_required_for_next_msg_size =
            bytes_forming_whole_messages + sizeof(message_header<generic_object>);

        if (recv_buff_.size() < bytes_required_for_next_msg_size) {
            return bytes_forming_whole_messages;
        }

        wl::Wmessage_size_t size_of_next_msg;
        const auto ptr_to_size = std::ranges::next(
            recv_buff_.begin(),
            bytes_forming_whole_messages + wl::message_header<wl::generic_object>::size_offset);
        std::memcpy(&size_of_next_msg, &*ptr_to_size, sizeof(wl::Wmessage_size_t));

        const auto bytes_potentially_forming_whole_messages =
            bytes_forming_whole_messages + size_of_next_msg.value;

        const auto potential_bytes_fit =
            recv_buff_.size() >= bytes_potentially_forming_whole_messages;

        if (potential_bytes_fit) {
            bytes_forming_whole_messages = bytes_potentially_forming_whole_messages;
            goto try_next_msg;
        }

        return bytes_forming_whole_messages;
    };

    recv_more_data();
    const auto bytes_to_parse =
        std::span<const std::byte>(recv_buff_.begin(),
                                   get_bytes_confirmed_to_form_whole_messages());

    auto parser_with_whole_messages = message_parser{ bytes_to_parse };

    recv_buff_.erase(recv_buff_.begin(),
                     std::ranges::next(recv_buff_.begin(), bytes_to_parse.size()));
    return parser_with_whole_messages;
}

} // namespace wl
} // namespace ger
