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

#include <boost/ut.hpp> // import boost.ut;

constexpr auto sum(auto... values) { return (values + ...); }

int main() {
    using namespace boost::ut;

    cfg<override> = { .tag = { "execute" } };

    tag("execute") / "sum"_test = [] {
        expect(sum(0) == 0_i);
        expect(sum(1, 2) == 3_i);
        expect(sum(1, 2) > 0_i and 42_i == sum(40, 2));
    };

    // Tag demonstration from:
    // https://github.com/boost-ext/ut/blob/master/example/tag.cpp
    //
    // Copyright (c) 2019-2020 Kris Jusiak (kris at jusiak dot net)
    //
    // Distributed under the Boost Software License, Version 1.0.
    // (See accompanying file LICENSE_1_0.txt or copy at
    // http://www.boost.org/LICENSE_1_0.txt)

    tag("execute") / skip / "tag demo 1: [execute][skip]"_test = [] {
        expect(42_i == 43) << "should not fire!";
        expect(false) << "should fail!";
    };

    tag("execute") / "tag demo 2: [execute]"_test = [] { expect(42_i == 42); };

    tag("not executed") / "tag demo 3: [not executed]"_test = [] { expect(43_i == 42); };

    tag("not executed") / tag("execute")
        / "tag demo 4: [not executed][executed]"_test = [] { expect(42_i == 42); };

    // -- tag demonstraion ends --
}
