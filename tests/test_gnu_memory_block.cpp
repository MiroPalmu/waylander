#include <config.h>

#include <boost/ut.hpp> // import boost.ut;

#include <algorithm>

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

    tag("gnu") / "memory_block can be mapped"_test = [] {
        gnu::memory_block mem{};
        constexpr auto size = 100uz;
        expect(nothrow([&] {
            mem.truncate(size);
            {
                const auto mapped = mem.map(size);
                std::ranges::fill(mapped, std::byte{ 42 });
            }
        }));

        const auto is42 = [](const std::byte x) { return x == std::byte{ 42 }; };
        expect(std::ranges::all_of(mem.map(size), is42));
    };
}
