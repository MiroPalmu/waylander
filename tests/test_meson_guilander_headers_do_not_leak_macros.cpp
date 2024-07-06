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
/// This test file assumes that this executable is compiled with meson as follows:
///
///     executable(
///         "...",
///         files(<__FILE__>),
///         dependencies: [<boost_ut_dep>, <guilander_dep>],
///         ...
///     )

#include "gnu_utils/pipe.hpp"

int main() {
    using namespace boost::ut;
    cfg<override> = { .tag = { "meson" } };

    /// Heuristic for tests that guilander headers do not leak some of gnulib macros.
    tag("meson") / "gnu_utils/pipe.hpp does not define write or read macros"_test = [] {
#ifdef write
        expect(false);
#else
        expect(true);
#endif

#ifdef read
        expect(false);
#else
        expect(true);
#endif
    };
}
