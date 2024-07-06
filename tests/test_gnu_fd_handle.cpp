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

#include <boost/ut.hpp> // import boost.ut;

/// @file
/// As fd_handle is not uspposed to be used on its own,
/// it is not directly tested any more that is here.
/// However other functionality is tested indirectly by
/// tests of functionality which are implemented using fd_handle.

#include <concepts>

#include "gnu_utils/fd_handle.hpp"

int main() {
    using namespace boost::ut;
    cfg<override> = { .tag = { "gnu" } };

    tag("gnu") / "standalone fd_handle can not be constructed"_test = [] {
        expect(not std::default_initializable<ger::gnu::fd_handle>);
    };

    tag("gnu") / "fd_handle is movable but not copyable"_test = [] {
        expect(std::movable<ger::gnu::fd_handle>);
        expect(not std::copyable<ger::gnu::fd_handle>);
    };

    tag("gnu") / "fd_ref is movable and copyable"_test = [] {
        expect(std::movable<ger::gnu::fd_ref>);
        expect(std::copyable<ger::gnu::fd_ref>);
    };

    tag("gnu") / "fd_ref can be constructed from fd_handle"_test = [] {
        expect(requires(ger::gnu::fd_handle fd) { ger::gnu::fd_ref{ fd }; });
    };
}
