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
