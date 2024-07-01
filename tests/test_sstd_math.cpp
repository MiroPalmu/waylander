#include <config.h>

#include <boost/ut.hpp> // import boost.ut;

#include "sstd.hpp"


int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "sstd" } };

    tag("sstd") / "round_upto_multiple_of<4>(N) is correct for N < 10"_test = [] {
        using namespace boost::ut::literals;
        expect(0_i == sstd::round_upto_multiple_of<4>(0));
        expect(3_i == sstd::round_upto_multiple_of<4>(1));
        expect(2_i == sstd::round_upto_multiple_of<4>(2));
        expect(1_i == sstd::round_upto_multiple_of<4>(3));
        expect(0_i == sstd::round_upto_multiple_of<4>(4));
        expect(3_i == sstd::round_upto_multiple_of<4>(5));
        expect(2_i == sstd::round_upto_multiple_of<4>(6));
        expect(1_i == sstd::round_upto_multiple_of<4>(7));
        expect(0_i == sstd::round_upto_multiple_of<4>(8));
        expect(3_i == sstd::round_upto_multiple_of<4>(9));
    };

}
