#include <config.h>

#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <print>
#include <unistd.h>

#include "linux_utils/file_descriptor.hpp"

namespace ger {
namespace linux {

[[nodiscard]] fd_type::fd_type() = default;
[[nodiscard]] fd_type::fd_type(fd_native_type&& fd) : fd_{ fd } {};

fd_type::~fd_type() {
    if (not fd_) return;

    if (-1 == close(fd_.value())) {
        std::println(stderr, "closing fd {} failed with errno: {}", fd_.value(), errno);
        std::println(stderr, "exiting...");
        std::exit(errno);
    }
}

fd_type::fd_type(fd_type&& rhs) {
    fd_ = std::move(rhs.fd_);
    rhs.fd_.reset();
};

fd_type& fd_type::operator=(fd_type&& rhs) {
    fd_ = std::move(rhs.fd_);
    rhs.fd_.reset();
    return *this;
};

} // namespace linux
} // namespace ger
