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
