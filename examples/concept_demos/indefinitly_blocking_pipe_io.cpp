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

#include <array>
#include <csignal>
#include <cstddef>
#include <print>
#include <span>

#include "gnu_utils/pipe.hpp"

int main() {
    std::signal(SIGINT, [](int) { std::println("SIGINT is ignored."); });

    [[maybe_unused]] auto [in, out] = ger::gnu::open_pipe();
    auto some_data                  = std::array<std::byte, 5>{};

    std::print("Reading... ");
    // Read should retry after handeled signal.
    const auto _ = in.read(some_data);
    std::println("Done!");
}
