#include <config.h>

/// Required to make gnulib not define macros for libc functions.
///
/// For more information see guilander sphinx docs: Usage of Gnulib.
#define GNULIB_NAMESPACE gnulib
#include "unistd-safer.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <algorithm>
#include <climits>
#include <format>
#include <random>
#include <ranges>
#include <stdexcept>
#include <string>

#include "gnu_utils/fd_handle.hpp"
#include "gnu_utils/memory_block.hpp"
#include "sstd.hpp"

namespace {

[[nodiscard]] auto random_shm_name() -> std::string {
    constexpr auto chars =
        std::array{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
                    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
                    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

    auto mt   = std::mt19937_64{ std::random_device{}() };
    auto dist = std::uniform_int_distribution<std::size_t>{ 0, std::ranges::size(chars) - 1 };
    auto random_char = [&] { return chars[dist(mt)]; };

    auto name = std::string{};
    std::ranges::generate_n(std::back_inserter(name), NAME_MAX - 1, random_char);
    return name;
}

[[nodiscard]] auto open_anon_shm_fd() -> ger::gnu::fd_native_type {
    constexpr auto max_tries = 10;
    for (const auto _ : std::ranges::views::iota(0, max_tries)) {
        const auto name_candidate = random_shm_name();

        constexpr auto create_exclusive_rw = O_RDWR | O_CREAT | O_EXCL;
        constexpr auto owner_rw            = S_IRUSR | S_IWUSR;
        auto fd = shm_open(name_candidate.c_str(), create_exclusive_rw, owner_rw);
        if (fd == -1) {
            if (errno == EEXIST) {
                // shm memory already exits with the same name -> try again.
                continue;
            }
            ger::sstd::throw_generic_system_error();
        }
        fd = fd_safer_flag(fd, O_CLOEXEC);
        if (fd == -1) {
            ger::sstd::throw_generic_system_error();
        } else {
            // Make successfully created shm_fd anonimous.
            if (-1 == shm_unlink(name_candidate.c_str())) {
                ger::sstd::throw_generic_system_error();
            }
            return fd;
        }
        std::unreachable();
    }
    throw std::runtime_error{
        std::format("Could not create POSIX shm object with unique name after {} times.", max_tries)
    };
}
} // namespace

[[nodiscard]] ger::gnu::memory_block::memory_block() : memory_block_fd{ open_anon_shm_fd() } {}
