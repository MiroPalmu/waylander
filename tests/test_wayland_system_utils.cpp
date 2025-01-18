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


#include <boost/ut.hpp> // import boost.ut;

#include "stddef.h"

#include <cstdlib>
#include <filesystem>
#include <format>
#include <stdexcept>
#include <string>
#include <tuple>

#include "guilander/sstd.hpp"
#include "guilander/wayland/system_utils.hpp"

namespace testing {

/// Testing utility to set env variable \p name to \p val.
auto setenv(const std::string& name, const std::string& val) {
    constexpr auto override_if_present = 1;
    const auto setenv_failed           = ::setenv(name.c_str(), val.c_str(), override_if_present);
    if (setenv_failed) { ger::sstd::throw_generic_system_error(); }
}

/// Testing utility to remove env variable \p name.
auto unsetenv(const std::string& name) {
    const auto unsetenv_failed = ::unsetenv(name.c_str());
    if (unsetenv_failed) { ger::sstd::throw_generic_system_error(); }
}

} // namespace testing

int main() {
    using namespace boost::ut;
    using namespace ger;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "env variable testing utility"_test = [] {
        const auto name  = std::string{ "some_long_testing_name" };
        const auto value = std::string{ "some_long_testing_value" };

        // Basic setting works:

        testing::setenv(name, value);

        const auto A = std::getenv(name.c_str());
        if (A == nullptr) {
            expect(false) << std::format("std::getenv{} returned nullptr", name);
            return;
        }

        expect(value == A);

        // Overriding works:

        testing::setenv(name, value + value);

        const auto B = std::getenv(name.c_str());

        if (B == nullptr) {
            expect(false) << std::format("std::getenv{} returned nullptr", name);
            return;
        }

        expect(value + value == B);

        // Usetting works:

        testing::unsetenv(name);

        expect(nullptr == std::getenv(name.c_str()));
    };

    wl_tag / "wayland_socket_path uses WAYLAND_DISPLAY if it is a absolute path"_test = [] {
        const auto relative_path = std::filesystem::path{ "some/relative/path/to/wl.sock" };
        const auto absolute_path = std::filesystem::path{ "/some/absolute/path/to/wl.sock" };

        expect(relative_path.is_relative());
        expect(absolute_path.is_absolute());

        testing::setenv("WAYLAND_DISPLAY", relative_path);
        expect(wl::wayland_socket_path() != relative_path);

        testing::setenv("WAYLAND_DISPLAY", absolute_path);
        expect(wl::wayland_socket_path() == absolute_path);
    };

    wl_tag
        / "wayland_socket_path throws if WAYLAND_DISPLAY and XDG_RUNTIME_DIR are not present"_test =
        [] {
            testing::unsetenv("WAYLAND_DISPLAY");
            testing::unsetenv("XDG_RUNTIME_DIR");
            expect(std::getenv("WAYLAND_DISPLAY") == nullptr);
            expect(std::getenv("XDG_RUNTIME_DIR") == nullptr);

            expect(throws<std::runtime_error>([] { std::ignore = wl::wayland_socket_path(); }));
        };

    wl_tag
        / "wayland_socket_path throws if WAYLAND_DISPLAY is relative and XDG_RUNTIME_DIR are not present"_test =
        [] {
            const auto relative_path = std::filesystem::path{ "some/relative/path/to/wl.sock" };
            expect(relative_path.is_relative());
            testing::setenv("WAYLAND_DISPLAY", relative_path);

            testing::unsetenv("XDG_RUNTIME_DIR");
            expect(std::getenv("XDG_RUNTIME_DIR") == nullptr);

            expect(throws<std::runtime_error>([] { std::ignore = wl::wayland_socket_path(); }));
        };

    wl_tag
        / "wayland_socket_path is XDG_RUNTIME_DIR/WAYLAND_DISPLAY if WAYLAND_DISPLAY is relative"_test =
        [] {
            const auto relative_path = std::filesystem::path{ "some/relative/path/to/wl.sock" };
            expect(relative_path.is_relative());
            testing::setenv("WAYLAND_DISPLAY", relative_path);

            const auto runtime_path = std::filesystem::path{ "/some/runtime/path" };
            testing::setenv("XDG_RUNTIME_DIR", runtime_path);

            expect(wl::wayland_socket_path() == runtime_path / relative_path);
        };

    wl_tag
        / "wayland_socket_path is XDG_RUNTIME_DIR/wayland-0 if WAYLAND_DISPLAY is not present"_test =
        [] {
            const auto runtime_path = std::filesystem::path{ "/some/runtime/path" };
            testing::setenv("XDG_RUNTIME_DIR", runtime_path);

            testing::unsetenv("WAYLAND_DISPLAY");
            expect(std::getenv("WAYLAND_DISPLAY") == nullptr);

            expect(wl::wayland_socket_path() == runtime_path / "wayland-0");
        };
};
