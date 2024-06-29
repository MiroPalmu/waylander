#pragma once

#include <cstddef>
#include <filesystem>
#include <span>
#include <tuple>

#include "sstd.hpp"

namespace ger {
namespace gnu {

// Forward declerations
class in_pipe;
class out_pipe;
class local_stream_socket;

using fd_native_type             = int;
static constexpr auto invalid_fd = fd_native_type{ -1 };

/// Unmaps memory specified by a span. Throws on failure.
void unmap(const std::span<std::byte>);
using mapped_memory = sstd::unique_handle<std::span<std::byte>, unmap>;

/// Throwing wrapper for executing libc calls on file descriptors.
///
/// Does not...
///     - ...own any system resources
///     - ...implement raii semantics
///     - ...validate contained fd_native_type before calls
///     - ...have any invariants
///
/// Flags set by default:
///     - O_CLOEXEC
///
/// Flags cleared by default:
///     - O_NONBLOCK
///
/// At the moment there is no facilities implemented to enable any of the flags above,
/// nor it is a priority goal.
///
/// All interfaces, that opens new file descriptions, ensures that any file descriptor opened
/// is not stdin(0), stdout(1) nor stderr(2). This perevents accidental globbing of them when for
/// variaty of reasons they might be closed and new file descriptors are allocated to
/// "the smallest non-negative integer that does not yet correspond to an open file descriptor".
///
/// For more information see gnulib manual 17.10 Handling closed standard file descriptors.
class fd_handle {
    fd_native_type fd_{ invalid_fd };

    [[nodiscard]] fd_handle(const fd_native_type fd);

  public:
    /// Blocking write.
    ///
    /// Writes data specified by \p what_to_write to fd.
    ///
    /// Keeps retrying if partial write happens or if writing is interrupted by signal (EINTR).
    ///
    /// Throws excpetion in case of error. Note that despite retrying efforts,
    /// partial writes might still occure, as error can happen during retry
    /// after partial write, which leads to exception and not continuation of retries.
    void write(const std::span<const std::byte> what_to_write);

    /// Blocking read.
    ///
    /// Writes data read from fd to memory specified by \p where_to_read,
    /// until it is full or EOF is encountered. Returns amount of bytes read.
    ///
    /// Keeps retrying if partial read happens or if reading is interrupted by signal (EINTR),
    /// until all asked bytes are read or EOF in encountered, which is indicated
    /// by returned amount of read bytes being less than what was requested.
    ///
    /// Throws excpetion in case of error. Note that despite retrying efforts,
    /// partial reads might still occure, as error can happen during retry
    /// after partial read, which leads to exception and not continuation of retries.
    [[nodiscard]] auto read(const std::span<std::byte> where_to_read) -> std::size_t;

    /// Blocking read, but does not retry on partial read (cf. fd_handle::read).
    ///
    /// Writes data read from fd to memory specified by \p where_to_read,
    /// retrying if interrupted. Returns the actual number of bytes read,
    /// zero for EOF and throws if error occurs.
    [[nodiscard]] auto read_some(const std::span<std::byte> where_to_read) -> std::size_t;

    /// Calls ftruncate(...) on the file descriptors. Throws on failure.
    void truncate(const std::size_t);

    /// Maps \p length bytes from beginning of the file descriptor as non-executable shared memory.
    [[nodiscard]] auto map(const std::size_t length) -> mapped_memory;

    /// Opens posix pipe (i.e. file descriptor IN for reading and OUT for writing).
    ///
    /// From returned tuple fd_handle at index 0 is the read end of the pipe (IN) and
    /// fd_handle at index 1 is the write end of the pipe (OUT).
    ///
    ///     > An easy way to remember that the input end comes first is that
    ///     > file descriptor 0 is standard input,
    ///     > and file descriptor 1 is standard output
    ///                                           GNU libc manual 15.1 Creating a Pipe
    ///
    /// Setting flags (importantly O_CLOEXEC) is generally not atomic with opening of the pipe.
    /// This matters in multi-threaded programs that spawn child processes.
    /// See gnulib manual 13.116.24 pipe2 for more information.
    friend auto open_pipe() -> std::tuple<in_pipe, out_pipe>;

    /// Opens pair of streaming style sockets in AF_LOCAL namespace, connected to each other.
    friend auto open_local_stream_socket_pair()
        -> std::pair<local_stream_socket, local_stream_socket>;

    /// Connect to socket at path. Throws if connection fails.
    friend auto open_local_stream_socket_connected_to(const std::filesystem::path&)
        -> local_stream_socket;

    friend class memory_block;

    /// Maximum number of bytes that is guaranteed to be atomic when writing to a pipe.
    ///
    /// If PIPE_BUF is defined return it, else return fpathconf(fd_, PIPE_BUF),
    /// as POSIX 2018 defines:
    ///
    ///  > A definition of one of the symbolic constants in the following list
    ///  > SHALL BE OMITTED from the <limits.h> header on specific implementations
    ///  > where the corresponding value is equal to or greater than the stated minimum,
    ///  > but where the value can vary depending on the file to which it is applied.
    ///
    /// Throws on error from fpatconf(...).
    [[nodiscard]] auto get_PIPE_BUF() const -> unsigned long;

    /// Closes the file descriptor.
    ///
    /// Throws if closing failed due to error.
    void close();

    /// As fd_handle does not own any system resources, dtor does nothing.
    ~fd_handle() = default;

    // Moveable:
    [[nodiscard]] fd_handle(fd_handle&&);
    fd_handle& operator=(fd_handle&&);
    // Not copyable:
    fd_handle(const fd_handle&)            = delete;
    fd_handle& operator=(const fd_handle&) = delete;
};

/// Simple closer function, which just calls \p x.close().
void close_fd_handle(fd_handle& x);

} // namespace gnu
} // namespace ger
