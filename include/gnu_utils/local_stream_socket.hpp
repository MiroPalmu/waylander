#pragma once

#define GNULIB_NAMESPACE gnulib
/// Assume config.h is already included where this header is used.
#include <sys/socket.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <filesystem>
#include <iterator>
#include <memory>
#include <ranges>
#include <tuple>
#include <type_traits>

#include "gnu_utils/fd_handle.hpp"
#include "sstd.hpp"
#include "type_utils.hpp"

#if defined CMSG_FIRSTHDR && !defined __sgi
#elif HAVE_STRUCT_MSGHDR_MSG_ACCRIGHTS
static_assert(false, "Implementation of gnulib module passfd supports this but not ger::gnu.");
#else
static_assert(false);
#endif

namespace ger {
namespace gnu {

template<typename>
struct is_valid_local_socket_msg_ctor_arg : std::false_type {};

template<std::size_t n>
struct is_valid_local_socket_msg_ctor_arg<std::span<std::byte, n>> : std::true_type {};
template<>
struct is_valid_local_socket_msg_ctor_arg<gnu::fd_ref> : std::true_type {};

/// Message type to specify send/recv buffers and ancillary data.
///
/// NOTE: There is not one-to-one relation between fd_slots and how many file descriptors
/// message object can actually receive, due to [1]:
///
/// > The CMSG_SPACE() macro is intended to help you decide how much space to allocate
/// > to receive an ancillary message. It rounds up its calculation to the next word boundary.
/// > Unfortunately, on 64-bit systems, this means you will always end up with enough space
/// > for an even number of file descriptors. If you were expecting just one FD,
/// > you'll end up with enough buffer space to receive two.
///
/// So it has to be specified what to do with the file descriptors, that are outside
/// of the expected "slots" but due to CMSG_SPACE shenanigans still fit into the cmsg buffer.
/// Currently this situation is treated as if they were truncated with MSG_CTRUNC.
///
/// [1] https://gist.github.com/kentonv/bc7592af98c68ba2738f4436920868dc?permalink_comment_id=2876346#gistcomment-2876346
template<std::size_t buff_slots, std::size_t fd_slots = 0>
    requires(buff_slots != 0)
class local_socket_msg {
    static constexpr auto cmsg_buff_len_ =
        (fd_slots > 0) ? CMSG_SPACE(fd_slots * sizeof(fd_native_type)) : 0;

    using cmsg_buff_type = std::array<std::byte, cmsg_buff_len_>;

    /// If fd_arr_len == 0 the this does not have to take the required 1 byte of empty object.
    [[no_unique_address]] alignas(::cmsghdr) cmsg_buff_type cmsg_buff_{};
    std::array<::iovec, buff_slots> iovecs_;
    ::msghdr native_msg_;

    friend class local_stream_socket;

  public:
    template<typename... T>
        requires(sizeof...(T) != 0)
                and sstd::unary_predicate_trait_fold<is_valid_local_socket_msg_ctor_arg, T...>
    [[nodiscard]] explicit local_socket_msg(const T&... args) {
        using arg_type_list = sstd::type_list<T...>;

        using addition =
            decltype([](auto Lhs,
                        auto Rhs) -> sstd::numeral_t<decltype(Lhs)::value + decltype(Rhs)::value> {
                return {};
            });

        using buff_type = std::span<std::byte>;
        // clang-format off
        constexpr auto num_of_span_args = arg_type_list
            ::template map<sstd::numeral_t<1>(buff_type), sstd::numeral_t<0>(gnu::fd_ref)>
            ::template fold_left<sstd::numeral_t<0>, addition>
            ::value;

        constexpr auto num_of_fd_args = arg_type_list
            ::template map<sstd::numeral_t<0>(buff_type), sstd::numeral_t<1>(gnu::fd_ref)>
            ::template fold_left<sstd::numeral_t<0>, addition>
            ::value;
        // clang-format on

        static_assert(num_of_span_args == buff_slots);
        static_assert(num_of_fd_args == fd_slots or num_of_fd_args == 0,
                      "Either give fd to all fd slots or give none in anticipation of send.");

        native_msg_.msg_name       = nullptr;
        native_msg_.msg_namelen    = 0;
        native_msg_.msg_iov        = iovecs_.data();
        native_msg_.msg_iovlen     = iovecs_.size();
        native_msg_.msg_control    = nullptr;
        native_msg_.msg_controllen = 0;
        native_msg_.msg_flags      = {};

        if constexpr (fd_slots != 0) {
            auto* const cmsg =
                std::construct_at<::cmsghdr>(reinterpret_cast<::cmsghdr*>(cmsg_buff_.data()));
            cmsg->cmsg_level = SOL_SOCKET;
            cmsg->cmsg_type  = SCM_RIGHTS;
            cmsg->cmsg_len   = CMSG_LEN(fd_slots * sizeof(fd_native_type));

            native_msg_.msg_control    = cmsg;
            native_msg_.msg_controllen = cmsg_buff_.size();
        }

        auto handle_arg = sstd::overloaded{
            [&, next_iovec = iovecs_.begin()]<std::size_t buff_len>(
                const std::span<std::byte, buff_len>& buff) mutable {
                *(next_iovec++) = ::iovec{ .iov_base{ buff.data() }, .iov_len{ buff.size() } };
            },
            [&, next_fd = CMSG_DATA(static_cast<::cmsghdr*>(native_msg_.msg_control))](
                const gnu::fd_ref fd) mutable {
                std::memcpy(next_fd, std::addressof(fd.fd_), sizeof(fd_native_type));
                std::ranges::advance(next_fd, sizeof(fd_native_type));
            }
        };

        [&]<std::size_t... I>(std::index_sequence<I...>) {
            (handle_arg(args), ...);
        }(std::index_sequence_for<T...>{});
    }

    /// Construct a fd_handles from ones at fd slots.
    ///
    /// Throws and closes all file descriptors if some of them were truncated.
    [[nodiscard]] auto get_fd_handles() -> sstd::
        template_invoke_with_repeated_argument_t<std::tuple, std::optional<fd_handle>, fd_slots>
        requires(fd_slots != 0)
    {
        const auto* const cmsg = CMSG_FIRSTHDR(&native_msg_);
        if (cmsg == nullptr) { return {}; }

        /* be paranoiac */
        if (cmsg->cmsg_level != SOL_SOCKET || cmsg->cmsg_type != SCM_RIGHTS) {
            throw std::runtime_error{ "Unregonized UNIX domain (local) socket ancillary data." };
        }

        auto next_fd = CMSG_DATA(static_cast<::cmsghdr*>(native_msg_.msg_control));
        if (next_fd == nullptr or cmsg->cmsg_len == 0) { return {}; }

        const auto amount_of_fds = [&] {
            // ATM next_fd points to beginning of the cmsg data buffer.
            const auto cmsg_header_size =
                std::ranges::distance(reinterpret_cast<const std::byte*>(cmsg),
                                      reinterpret_cast<const std::byte*>(next_fd));
            const auto actual_cmsg_buff_size = cmsg->cmsg_len - cmsg_header_size;
            const auto num_fd                = actual_cmsg_buff_size / sizeof(fd_native_type);
            const auto leftover              = actual_cmsg_buff_size % sizeof(fd_native_type);

            if (leftover != 0) {
                throw std::runtime_error{
                    "Failed to deduce number of file descriptors in UNIX domain (local) socket "
                    "SCM_RIGHTS ancillary data."
                };
            }

            return num_fd;
        }();

        // N := 0, 1, ...
        const auto get_Nth_fd = [&](const std::size_t N) -> std::optional<fd_handle> {
            if (N >= amount_of_fds) { return {}; }

            fd_native_type native_fd;
            std::memcpy(&native_fd, next_fd, sizeof(fd_native_type));
            std::ranges::advance(next_fd, sizeof(fd_native_type));

            return fd_handle{ native_fd };
        };

        if (amount_of_fds > fd_slots) {
            // Fake truncatio when more file descriptors than fd slots.
            native_msg_.msg_flags |= MSG_CTRUNC;
        }

        if (native_msg_.msg_flags & MSG_CTRUNC) {
            for (const auto n : std::views::iota(0uz) | std::views::take(amount_of_fds)) {
                get_Nth_fd(n).value().close();
            }
            throw std::domain_error{
                "Truncated UNIX domain (local) socket SCM_RIGHTS ancillary data."
            };
        }

        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::tuple{ get_Nth_fd(I)... };
        }(std::make_index_sequence<fd_slots>{});
    };
};

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

    /// Sends message represented by local_socket_msg<...>.
    ///
    /// Blocks until at least one byte is send. Returns the total bytes send.
    template<std::size_t buff_slots, std::size_t fd_slots>
    auto send(local_socket_msg<buff_slots, fd_slots>& msg) -> std::size_t {
        msg.native_msg_.msg_flags = {};
    try_again:
        const auto x = ::sendmsg(fd_, &msg.native_msg_, 0);
        if (x == -1) {
            if (errno == EINTR) {
                // Signals..., no data transmitted.
                goto try_again;
            }
            sstd::throw_generic_system_error();
        }
        return x;
    }

    /// Receives message indicated by given local_socket_msg<...>.
    ///
    /// Blocks until at least one byte is received. Returns the number of bytes received.
    template<std::size_t buff_arr_len, std::size_t fd_arr_len>
    auto recv(local_socket_msg<buff_arr_len, fd_arr_len>& msg) -> std::size_t {
        msg.native_msg_.msg_flags = MSG_CMSG_CLOEXEC;
    try_again:
        const auto x = ::recvmsg(fd_, &msg.native_msg_, 0);
        if (x == -1) {
            if (errno == EINTR) {
                // Signals..., no data transmitted.
                goto try_again;
            }
            sstd::throw_generic_system_error();
        }

        if (x == 0) {
            // Fake errno on closed connection.
            errno = ENOTCONN;
            sstd::throw_generic_system_error();
        }

        return x;
    }
};

[[nodiscard]] auto open_local_stream_socket_pair()
    -> std::pair<local_stream_socket, local_stream_socket>;

[[nodiscard]] auto open_local_stream_socket_connected_to(const std::filesystem::path&)
    -> local_stream_socket;

} // namespace gnu
} // namespace ger
