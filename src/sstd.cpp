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

#include <config.h>

#include <cerrno>
#include <cstring>
#include <format>
#include <source_location>
#include <system_error>

#include "sstd.hpp"

namespace ger {
namespace sstd {

[[noreturn]] void throw_generic_system_error(const std::source_location loc) {
    const auto current_errno = errno;
    const auto err_msg       = std::format("{}:{}:{}: failed with error: {}",
                                     loc.file_name(),
                                     loc.function_name(),
                                     loc.line(),
                                     std::strerror(current_errno));
    throw std::system_error(current_errno, std::generic_category(), std::strerror(current_errno));
};

[[noreturn]] void throw_partial_system_io_error(const std::size_t transfered_bytes,
                                                const std::size_t total_bytes,
                                                const std::source_location loc) {
    const auto current_errno = errno;
    const auto err_msg       = std::format("{} failed and transfered {} out of {} total bytes:\n{}",
                                     loc.function_name(),
                                     transfered_bytes,
                                     total_bytes,
                                     std::strerror(current_errno));
    throw std::system_error(current_errno, std::generic_category(), err_msg);
}

} // namespace sstd
} // namespace ger
