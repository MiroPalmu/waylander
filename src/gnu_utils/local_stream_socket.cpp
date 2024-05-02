#include "sstd.hpp"
#include <config.h>
#include <fcntl.h>

/// Required to make gnulib not define macros for libc functions.
///
/// For more information see guilander sphinx docs: Usage of Gnulib.
#define GNULIB_NAMESPACE gnulib
/// For O_CLOEXEC macro and fd_safer_flag.
#include "unistd-safer.h"
#include <sys/socket.h>

#include <tuple>

#include "gnu_utils/fd_handle.hpp"
#include "gnu_utils/local_stream_socket.hpp"

namespace ger {
namespace gnu {

[[nodiscard]] auto open_local_stream_socket_pair()
    -> std::pair<local_stream_socket, local_stream_socket> {
    std::array<fd_native_type, 2> raw_fds;

    if (-1 == socketpair(PF_LOCAL, SOCK_STREAM, 0, raw_fds.data())) {
        // Opening failure.
        sstd::throw_generic_system_error();
    }

    // Prevent std{in,out,err} globbing and set O_CLOEXEC.
    for (auto& fd : raw_fds) {
        fd = fd_safer_flag(fd, O_CLOEXEC);
        if (fd == -1) sstd::throw_generic_system_error();
    }

    return { fd_handle{ raw_fds.front() }, fd_handle{ raw_fds.back() } };
}

} // namespace gnu
} // namespace ger
