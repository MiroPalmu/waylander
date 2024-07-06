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
