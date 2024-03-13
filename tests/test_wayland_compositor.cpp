#include <boost/ut.hpp> // import boost.ut;

#include "wayland/protocol.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger::wl;

    tag("wayland") / "compositor object can be constructor"_test = [] {
        expect(nothrow([] { auto _ = compositor(); }));
    };
}
