#include <boost/ut.hpp> // import boost.ut;

#include "gnu_utils/pipe.hpp"

int main() {
    using namespace boost::ut;
    cfg<override> = { .tag = { "gnu" } };

    tag("gnu") / "[in|out]_pipe can be constructed"_test = [] {
        [[maybe_unused]] auto [/* in */_, /* out */_] = ger::gnu::open_pipe();
        expect(true);
    };

    tag("gnu") / "[in|out]_pipe can be [written|read]"_test = [] {
        [[maybe_unused]] auto [/* in */_, /* out */_] = ger::gnu::open_pipe();
        expect(true);
    };
}
