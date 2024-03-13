#include <boost/ut.hpp> // import boost.ut;

#include "linux_utils/shm_memory.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger::linux;
    using namespace std;

    tag("linux") / "shm_mem can be constrcted"_test = [] {
        auto _ = shm_mem(10);
        expect(true);
    };
}
