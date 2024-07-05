#include <config.h>

// Required to make gnulib not define macros for libc functions.
//
// For more information see guilander sphinx docs: Usage of Gnulib.
#define GNULIB_NAMESPACE gnulib
// For O_CLOEXEC macro.
#include "unistd-safer.h"
#include <fcntl.h>

#include <tuple>

#include "gnu_utils/fd_handle.hpp"
#include "gnu_utils/pipe.hpp"

namespace ger {
namespace gnu {

[[nodiscard]] auto open_pipe() -> std::tuple<in_pipe, out_pipe> {
    std::array<fd_native_type, 2> raw_pipe_fd;

    // Each case is assumed to exit this function.
    switch (::pipe2_safer(raw_pipe_fd.data(), O_CLOEXEC)) {
        case 0: return { fd_handle{ raw_pipe_fd.front() }, fd_handle{ raw_pipe_fd.back() } };
        case -1: sstd::throw_generic_system_error();
    }
    std::unreachable();
}

} // namespace gnu
} // namespace ger
