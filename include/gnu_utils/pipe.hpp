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

#include <tuple>

#include "gnu_utils/fd_handle.hpp"
#include "sstd.hpp"

namespace ger {
namespace gnu {

class in_pipe;
class out_pipe;

using pipe_fd = sstd::unique_handle<fd_handle, close_fd_handle>;

class in_pipe : private pipe_fd {
    /// Inherit the move ctor of pipe_fd, i.e. pipe_fd::pipe_fd(fd_handle&&).
    using pipe_fd::pipe_fd;
    friend auto open_pipe() -> std::tuple<in_pipe, out_pipe>;

  public:
    using pipe_fd::read;
    using pipe_fd::get_PIPE_BUF;
};

class out_pipe : private pipe_fd {
    /// Inherit the move ctor of pipe_fd, i.e. pipe_fd::pipe_fd(fd_handle&&).
    using pipe_fd::pipe_fd;
    friend auto open_pipe() -> std::tuple<in_pipe, out_pipe>;

  public:
    using pipe_fd::write;
    using pipe_fd::get_PIPE_BUF;
};

[[nodiscard]] auto open_pipe() -> std::tuple<in_pipe, out_pipe>;

} // namespace gnu
} // namespace ger
