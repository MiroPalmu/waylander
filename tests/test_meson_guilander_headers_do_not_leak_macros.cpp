#include <boost/ut.hpp> // import boost.ut;

/// @file This test file assumes that this executable is compiled with meson as follows:
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

    /// Heuristic for tests that guilander headers do not leak gnulib macros.
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
