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
    using memory_block_fd::operator gnu::fd_ref;
};

} // namespace gnu
} // namespace ger
