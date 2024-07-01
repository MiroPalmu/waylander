#include <config.h>

#include <boost/ut.hpp> // import boost.ut;

/// @file This test file assumes that this executable is compiled with meson as follows:
///
///     executable(
///         "...",
///         files(<__FILE__>),
///         dependencies: [<boost_ut_dep>, <guilander_dep>],
///         ...
///     )

#include <stddef.h>
#include <string_view>

#include "gnu_utils/pipe.hpp"

int main() {
    using namespace boost::ut;
    cfg<override> = { .tag = { "meson" } };

    tag("meson") / "depending on guilander_dep makes gnulib config.h to be found"_test = [] {
        using namespace std::literals;
#ifdef PACKAGE
        expect(PACKAGE == "gnulib-modules-to-meson"sv);
#else
        expect(false);
#endif
    };

    tag("meson") / "depending on guilander_dep makes gnulib headers to be found"_test = [] {
#if __has_include("full-read.h")
        expect(true);
#else
        expect(false) << "Did not have full-read.h.";
#endif

#if __has_include("full-write.h")
        expect(true);
#else
        expect(false) << "Did not have full-write.h.";
#endif
    };
}
