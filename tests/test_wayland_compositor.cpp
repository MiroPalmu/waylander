#include <boost/ut.hpp> // import boost.ut;

#include "wayland/protocol.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger::wl;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "compositor object can be constructor"_test = [] {
        expect(nothrow([] { auto _ = compositor(); }));
    };
}
