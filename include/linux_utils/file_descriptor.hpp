#pragma once

#ifndef __linux__
#    error "Programmed with assumption, that this error does not trigger."
#endif

#include <optional>

namespace ger {
namespace linux {

using fd_native_type = int;

class fd_type {
    std::optional<fd_native_type> fd_{};

  public:
    [[nodiscard]] fd_type();
    [[nodiscard]] fd_type(fd_native_type&& fd);
    ~fd_type();

    [[nodiscard]] constexpr operator fd_native_type(this auto&& self) { return self.fd_.value(); }

    // Moveable:
    fd_type(fd_type&& rhs);
    fd_type& operator=(fd_type&& rhs);
    // Not copyable:
    fd_type(const fd_type&)            = delete;
    fd_type& operator=(const fd_type&) = delete;
};

} // namespace linux
} // namespace ger
