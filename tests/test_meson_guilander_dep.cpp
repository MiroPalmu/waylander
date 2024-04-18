#include <boost/ut.hpp> // import boost.ut;

/// @file This test file assumes that this executable is compiled with meson as follows:
///
///     executable(
///         "...",
///         files(<__FILE__>),
///         dependencies: [<boost_ut_dep>, <guilander_dep>],
///         ...
///     )

// Include before potential config.h.
// This should error out if gnulib stddef.h is found and config.h is not include.
#include <stddef.h>

#if __has_include(<config.h>)
#    include <config.h>
#endif

#include "gnu_utils/pipe.hpp"

int main() {
    using namespace boost::ut;
    cfg<override> = { .tag = { "meson" } };

    /// If this file compiles it has not found the wrappers,
    /// as they would error out if is not included first config.h.
    tag("meson")
        / "depending on guilander_dep does not 'wrap' libc headers"_test = [] { expect(true); };

    tag("meson") / "depending on guilander_dep does not gnulib config.h to be found"_test = [] {
#ifdef PACKAGE
        expect(PACKAGE != "gnulib-modules-to-meson");
#else
        expect(true);
#endif
    };

    tag("meson") / "depending on guilander_dep does not make gnulib headers to be found"_test = [] {
#if __has_include("full-read.h")
        expect(false);
#elif __has_include("full-write.h")
        expect(false);
#else
        expect(true);
#endif
    };
}
