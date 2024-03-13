#pragma once

#ifndef __linux__
#    error "Programmed with assumption, that this error does not trigger."
#endif

#include <cstddef>
#include <cstddef>
#include <span>

#include <fcntl.h> /* For O_* constants */
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */

#include "linux_utils/file_descriptor.hpp"

namespace ger {
namespace linux {

/// Move only owner of POSIX shared memory object mapped to virtual memory.
///
/// Owns file descriptor to POSIX shared memory object mmapped
/// to virtual address space. At destruction the memory is unmapped
/// and file descriptor will be closed.
class shm_mem {
    fd_type fd_{};
    std::size_t size_{};
    void* data_ = nullptr;
 
  public:
    /// Create anonymous shm object.
    ///
    /// Anonymous shm object means that it is created with random name
    /// and the created file is unlinked immediatly after,
    /// leaving only the file descriptor.
    ///
    /// To create anonimous object random name has to be generated.
    /// If that name is in use, try 10 different random names.
    /// Throws if none of them is unique.
    [[nodiscard]] shm_mem(const std::size_t size);
    ~shm_mem();

    [[nodiscard]] operator std::span<std::byte>();

    [[nodiscard]] auto span() -> std::span<std::byte>;

    // Movable
    shm_mem(shm_mem&&)            = default;
    shm_mem& operator=(shm_mem&&) = default;
    // Not copyable:
    shm_mem(const shm_mem&)            = delete;
    shm_mem& operator=(const shm_mem&) = delete;
};

} // namespace linux
} // namespace ger
