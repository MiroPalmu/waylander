#pragma once

#include <cstddef>
#include <span>

#include "gnu_utils/fd_handle.hpp"
#include "sstd.hpp"

namespace ger {
namespace gnu {

using memory_block_fd = sstd::unique_handle<fd_handle, close_fd_handle>;

/// Block of memory.
class memory_block : private memory_block_fd {
  public:
    /// Create block of memory via shm_open(...).
    [[nodiscard]] explicit memory_block();

    using memory_block_fd::truncate;
    using memory_block_fd::map;
};

} // namespace gnu
} // namespace ger
