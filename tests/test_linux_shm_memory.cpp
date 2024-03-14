#include <boost/ut.hpp> // import boost.ut;

#include "linux_utils/shm_memory.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger::linux;
    cfg<override> = { .tag = { "linux" } };

    tag("linux") / "shm_mem can be constrcted"_test = [] {
        auto _ = shm_mem(10);
        expect(true);
    };

    tag("linux") / "shm_mem can accessed using span<std::byte>"_test = [] {
        auto mem = shm_mem(10);
        auto i   = 0uz;
        for (auto& x : std::span<std::byte>{ mem }) { x = static_cast<std::byte>(i++); }
        i = 0z;
        for (const auto& x : mem.span()) { expect(x == static_cast<std::byte>(i++)); }
    };
}
