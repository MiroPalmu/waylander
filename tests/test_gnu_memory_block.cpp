#include <boost/ut.hpp> // import boost.ut;

#include "gnu_utils/memory_block.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger;

    cfg<override> = { .tag = { "gnu" } };

    tag("gnu") / "memory_block can be constructed via default constructor"_test = [] {
        expect(nothrow([] { [[maybe_unused]] gnu::memory_block _{}; }));
    };

    tag("gnu") / "memory_block can be truncated"_test = [] {
        gnu::memory_block mem{};
        expect(nothrow([&] { mem.truncate(100uz); }));
        expect(nothrow([&] { mem.truncate(200uz); }));
        expect(nothrow([&] { mem.truncate(0uz); }));
    };
}
