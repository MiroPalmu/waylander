#include <config.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <format>
#include <iterator>
#include <print>
#include <random>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include <linux/limits.h>
#include <sys/types.h>
#include <unistd.h>

#include "linux_utils/shm_memory.hpp"
#include "sstd.hpp"

namespace ger {
namespace linux {

[[nodiscard]] auto random_name(std::size_t length = 255) -> std::string {
    static constexpr auto chars =
        std::array{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
                    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
                    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

    auto mt   = std::mt19937_64{ std::random_device{}() };
    auto dist = std::uniform_int_distribution<std::size_t>{ 0, std::ranges::size(chars) - 1 };
    auto random_char = [&] { return chars[dist(mt)]; };

    auto name = std::string{};
    std::ranges::generate_n(std::back_inserter(name), length, random_char);
    return name;
}

[[nodiscard]] shm_mem::shm_mem(const std::size_t size) : size_{ size } {
    auto [fd, used_name] = [] {
        constexpr auto max_tries = 10;
        for (const auto _ : std::ranges::views::iota(0, max_tries)) {
            auto random_name_candidate = random_name();

            auto fd_temp = sstd::legacy_construct<fd_type>(shm_open,
                                                           random_name_candidate.c_str(),
                                                           O_RDWR | O_CREAT | O_EXCL,
                                                           0600);

            if (fd_temp == -1) continue;
            return std::make_tuple(std::move(fd_temp), std::move(random_name_candidate));
        }
        throw std::runtime_error{ std::format(
            "could not create POSIX shm object with unique name after {} times",
            max_tries) };
    }();

    if (shm_unlink(used_name.c_str()) == -1) {
        throw std::runtime_error{ std::format("shm_unlink(\"{}\") failed", used_name) };
    }

    if (ftruncate(fd, size) == -1) {
        throw std::runtime_error{
            std::format("ftruncate({}, {}) failed", static_cast<int>(fd), size)
        };
    }

    data_ = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (data_ == MAP_FAILED) {
        throw std::runtime_error{ std::format(
            "mmap( nullptr, {}, ..., {}, 0) failed with errno: {}",
            size,
            static_cast<int>(fd),
            errno) };
    }

    fd_ = std::move(fd);
}

shm_mem::~shm_mem() {
    if (-1 == munmap(data_, size_)) {
        std::println(stderr, "munmap failed with errno: {}", errno);
        std::println(stderr, "exiting...");
        std::exit(errno);
    }
}

shm_mem::operator std::span<std::byte>() {
    if (data_ == nullptr)
        throw std::runtime_error{ "Accessing nullptr in shm_mem::operator std::span<std::byte>" };
    return { static_cast<std::byte*>(data_), size_ };
}

[[nodiscard]] auto shm_mem::span() -> std::span<std::byte> { return *this; }

} // namespace linux
} // namespace ger
