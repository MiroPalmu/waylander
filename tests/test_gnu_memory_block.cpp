#include <boost/ut.hpp> // import boost.ut;

#include "gnu_utils/memory_block.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger;

    cfg<override> = { .tag = { "gnu" } };

    tag("gnu") / "memory_block can be constructed via default constructor"_test = [] {
        expect(nothrow([] { [[maybe_unused]] gnu::memory_block _{}; }));
    };
}
