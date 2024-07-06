// Copyright (C) 2024 Miro Palmu.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file.  If not, see <https://www.gnu.org/licenses/>.

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
