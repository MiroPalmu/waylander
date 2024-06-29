#include <config.h>

/// Required to make gnulib not define macros for libc functions.
///
/// For more information see guilander sphinx docs: Usage of Gnulib.
#define GNULIB_NAMESPACE gnulib
#include "full-read.h"
#include "full-write.h"
#include "safe-read.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <format>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "gnu_utils/fd_handle.hpp"
#include "sstd.hpp"

namespace ger {
namespace gnu {

[[nodiscard]] fd_handle::fd_handle(const fd_native_type fd) : fd_{ fd } {}
[[nodiscard]] fd_handle::fd_handle(fd_handle&&) = default;
fd_handle& fd_handle::operator=(fd_handle&&)    = default;

void fd_handle::write(const std::span<const std::byte> what_to_write) {
    const auto number_of_bytes_written =
        ::full_write(fd_, what_to_write.data(), what_to_write.size());

    if (number_of_bytes_written < what_to_write.size()) {
        sstd::throw_partial_system_io_error(number_of_bytes_written, what_to_write.size());
    }
}

auto fd_handle::read(const std::span<std::byte> where_to_read) -> std::size_t {
    const auto number_of_bytes_read = ::full_read(fd_, where_to_read.data(), where_to_read.size());

    const auto at_EOF{ errno != 0 };
    if (number_of_bytes_read < where_to_read.size() and not at_EOF) {
        sstd::throw_partial_system_io_error(number_of_bytes_read, where_to_read.size());
    }

    return number_of_bytes_read;
}

auto fd_handle::read_some(const std::span<std::byte> where_to_read) -> std::size_t {
    const auto number_of_bytes_read = ::safe_read(fd_, where_to_read.data(), where_to_read.size());
    if (number_of_bytes_read == SAFE_READ_ERROR) { sstd::throw_generic_system_error(); }

    return number_of_bytes_read;
}

void fd_handle::close() {
    // Each case is assumed to exit this function.
    switch (gnulib::close(fd_)) {
        case 0: return;
        case -1: sstd::throw_generic_system_error();
    }
    std::unreachable();
}

auto fd_handle::get_PIPE_BUF() const -> unsigned long {
#ifdef PIPE_BUF
    return PIPE_BUF;
#endif

    // Has to reset errno, as to differentiate between rows 2 and 3:
    //
    // row | fpathconf(...) | errno before | * after | implies
    // ====|================|==============|=========|=============================
    //   1 |              c |            0 |       0 | limit e
    //   2 |             -1 |            0 |       e | error e
    //   3 |             -1 |            0 |       0 | system does not impose limit

    errno = 0;
    const long limit{ ::fpathconf(fd_, _PC_PIPE_BUF) };
    const auto row1{ limit != -1 };
    if (row1) return limit;
    const auto row2{ errno != 0 };
    if (row2) sstd::throw_generic_system_error();

    /// Row 3:
    throw std::logic_error("Should not happen as PIPE_BUF is not defined.");
}

void close_fd_handle(fd_handle& x) { x.close(); }

void fd_handle::truncate(const std::size_t size) {
    if (not std::in_range<off_t>(size)) {
        throw std::runtime_error{ std::format("Requested size {} does not fit into off_t.", size) };
    }
    if (-1 == gnulib::ftruncate(fd_, size)) { sstd::throw_generic_system_error(); }
}

void unmap(const std::span<std::byte> mem) {
    if (-1 == ::munmap(static_cast<void*>(mem.data()), mem.size())) {
        sstd::throw_generic_system_error();
    }
}

[[nodiscard]] auto fd_handle::map(const std::size_t length) -> mapped_memory {
    auto* const mapped = ::mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    if (mapped == MAP_FAILED) { sstd::throw_generic_system_error(); }
    return mapped_memory(std::span<std::byte>(static_cast<std::byte*>(mapped), length));
}

} // namespace gnu
} // namespace ger
