#include <config.h>

#include <boost/ut.hpp> // import boost.ut;

#include <cstddef>
#include <ranges>

#include "bit_fiddling.hpp"

static_assert(ger::bit::total_memory_usage_is<0>);
static_assert(ger::bit::total_memory_usage_is<1, std::uint8_t>);
static_assert(ger::bit::total_memory_usage_is<4, std::uint32_t>);
static_assert(ger::bit::total_memory_usage_is<8, std::uint64_t>);

static_assert(ger::bit::total_memory_usage_is<0>);
static_assert(ger::bit::total_memory_usage_is<2, std::uint8_t, std::uint8_t>);
static_assert(ger::bit::total_memory_usage_is<5, std::uint8_t, std::uint32_t>);
static_assert(ger::bit::total_memory_usage_is<9, std::uint8_t, std::uint64_t>);

static_assert(ger::bit::total_memory_usage_is<3, std::uint8_t, std::uint8_t, std::uint8_t>);
static_assert(ger::bit::total_memory_usage_is<6, std::uint8_t, std::uint32_t, std::uint8_t>);
static_assert(ger::bit::total_memory_usage_is<10, std::uint8_t, std::uint64_t, std::uint8_t>);

static_assert(not ger::bit::total_memory_usage_is<1>);
static_assert(not ger::bit::total_memory_usage_is<2, std::uint8_t>);
static_assert(not ger::bit::total_memory_usage_is<5, std::uint32_t>);
static_assert(not ger::bit::total_memory_usage_is<9, std::uint64_t>);

static_assert(not ger::bit::total_memory_usage_is<1>);
static_assert(not ger::bit::total_memory_usage_is<3, std::uint8_t, std::uint8_t>);
static_assert(not ger::bit::total_memory_usage_is<6, std::uint8_t, std::uint32_t>);
static_assert(not ger::bit::total_memory_usage_is<10, std::uint8_t, std::uint64_t>);

static_assert(not ger::bit::total_memory_usage_is<4, std::uint8_t, std::uint8_t, std::uint8_t>);
static_assert(not ger::bit::total_memory_usage_is<7, std::uint8_t, std::uint32_t, std::uint8_t>);
static_assert(not ger::bit::total_memory_usage_is<11, std::uint8_t, std::uint64_t, std::uint8_t>);

int main() {
    using namespace boost::ut;
    using namespace ger::bit;
    using namespace std;

    "byte_buff is equality_comparable"_test = [] {
        static_assert(equality_comparable<byte_buff<29>>);

        constexpr auto buf_a1 = byte_buff<3>(uint8_t{ 0 }, uint8_t{ 1 }, uint8_t{ 2 });
        constexpr auto buf_a2 = byte_buff<3>(uint8_t{ 0 }, uint8_t{ 1 }, uint8_t{ 2 });
        constexpr auto buf_a3 = byte_buff<3>(uint8_t{ 0 }, uint8_t{ 2 }, uint8_t{ 2 });

        expect(buf_a1 == buf_a2);
        expect(buf_a1 != buf_a3);
        expect(buf_a2 != buf_a3);
    };

    "byte_buff<0> is always equal to every other byte_buff<0>"_test = [] {
        static_assert(equality_comparable<byte_buff<0>>);

        constexpr auto buf_empty_a = byte_buff<0>{};
        constexpr auto buf_empty_b = byte_buff<0>{};

        expect(buf_empty_a == buf_empty_a);
        expect(buf_empty_a == buf_empty_b);
        expect(buf_empty_b == buf_empty_b);
    };

    "integers in integer message byte_buff constructor can alias each other"_test = [] {
        constexpr auto one          = uint8_t{ 1 };
        constexpr auto oneone       = static_cast<uint16_t>(one << 8 | one);
        constexpr auto oneoneoneone = static_cast<uint32_t>(oneone << 16 | oneone);

        constexpr auto buf_ones_a = byte_buff<4>(one, one, one, one);
        constexpr auto buf_ones_b = byte_buff<4>(oneone, oneone);
        constexpr auto buf_ones_c = byte_buff<4>(oneoneoneone);

        constexpr auto buf_ones_d = byte_buff<4>(oneone, one, one);
        constexpr auto buf_ones_e = byte_buff<4>(one, oneone, one);
        constexpr auto buf_ones_f = byte_buff<4>(one, one, oneone);

        // [a-c] together
        expect(buf_ones_a == buf_ones_b);
        expect(buf_ones_a == buf_ones_c);
        expect(buf_ones_b == buf_ones_c);

        // [d-f] together
        expect(buf_ones_d == buf_ones_e);
        expect(buf_ones_d == buf_ones_f);
        expect(buf_ones_e == buf_ones_f);

        // a with [d-f]
        expect(buf_ones_a == buf_ones_d);
        expect(buf_ones_a == buf_ones_e);
        expect(buf_ones_a == buf_ones_f);

        // b with [d-f]
        expect(buf_ones_b == buf_ones_d);
        expect(buf_ones_b == buf_ones_e);
        expect(buf_ones_b == buf_ones_f);

        // c with [d-f]
        expect(buf_ones_c == buf_ones_d);
        expect(buf_ones_c == buf_ones_e);
        expect(buf_ones_c == buf_ones_f);
    };

    "byte_buff constructor deduces size with cdat"_test = [] {
        constexpr auto one          = uint8_t{ 1 };
        constexpr auto oneone       = static_cast<uint16_t>(one << 8 | one);
        constexpr auto oneoneoneone = static_cast<uint32_t>(oneone << 16 | oneone);

        constexpr auto buf_ones_a = byte_buff(one, one, one, one);
        constexpr auto buf_ones_b = byte_buff(oneone, oneone);
        constexpr auto buf_ones_c = byte_buff(oneoneoneone);

        constexpr auto buf_ones_d = byte_buff(oneone, one, one);
        constexpr auto buf_ones_e = byte_buff(one, oneone, one);
        constexpr auto buf_ones_f = byte_buff(one, one, oneone);

        // [a-c] together
        expect(buf_ones_a == buf_ones_b);
        expect(buf_ones_a == buf_ones_c);
        expect(buf_ones_b == buf_ones_c);

        // [d-f] together
        expect(buf_ones_d == buf_ones_e);
        expect(buf_ones_d == buf_ones_f);
        expect(buf_ones_e == buf_ones_f);

        // a with [d-f]
        expect(buf_ones_a == buf_ones_d);
        expect(buf_ones_a == buf_ones_e);
        expect(buf_ones_a == buf_ones_f);

        // b with [d-f]
        expect(buf_ones_b == buf_ones_d);
        expect(buf_ones_b == buf_ones_e);
        expect(buf_ones_b == buf_ones_f);

        // c with [d-f]
        expect(buf_ones_c == buf_ones_d);
        expect(buf_ones_c == buf_ones_e);
        expect(buf_ones_c == buf_ones_f);
    };

    "span from byte_buff is equal to the original data"_test = [] {
        constexpr auto one          = uint8_t{ 1 };
        constexpr auto oneone       = static_cast<uint16_t>(one << 8 | one);
        constexpr auto oneoneoneone = static_cast<uint32_t>(oneone << 16 | oneone);

        constexpr auto buf_ones_a = byte_buff(one, one, one, one);
        constexpr auto buf_ones_b = byte_buff(oneone, oneone);
        constexpr auto buf_ones_c = byte_buff(oneoneoneone);

        constexpr auto buf_ones_d = byte_buff(oneone, one, one);
        constexpr auto buf_ones_e = byte_buff(one, oneone, one);
        constexpr auto buf_ones_f = byte_buff(one, one, oneone);

        // [a-c] together
        expect(std::ranges::equal(buf_ones_a.bytes(), buf_ones_b.bytes()));
        expect(std::ranges::equal(buf_ones_a.bytes(), buf_ones_c.bytes()));
        expect(std::ranges::equal(buf_ones_b.bytes(), buf_ones_c.bytes()));

        // [d-f] together
        expect(std::ranges::equal(buf_ones_d.bytes(), buf_ones_e.bytes()));
        expect(std::ranges::equal(buf_ones_d.bytes(), buf_ones_f.bytes()));
        expect(std::ranges::equal(buf_ones_e.bytes(), buf_ones_f.bytes()));

        // a with [d-f]
        expect(std::ranges::equal(buf_ones_a.bytes(), buf_ones_d.bytes()));
        expect(std::ranges::equal(buf_ones_a.bytes(), buf_ones_e.bytes()));
        expect(std::ranges::equal(buf_ones_a.bytes(), buf_ones_f.bytes()));

        // b with [d-f]
        expect(std::ranges::equal(buf_ones_b.bytes(), buf_ones_d.bytes()));
        expect(std::ranges::equal(buf_ones_b.bytes(), buf_ones_e.bytes()));
        expect(std::ranges::equal(buf_ones_b.bytes(), buf_ones_f.bytes()));

        // c with [d-f]
        expect(std::ranges::equal(buf_ones_c.bytes(), buf_ones_d.bytes()));
        expect(std::ranges::equal(buf_ones_c.bytes(), buf_ones_e.bytes()));
        expect(std::ranges::equal(buf_ones_c.bytes(), buf_ones_f.bytes()));
    };

    "byte_buff can be constructed from trivially copyable types during runtime"_test = [] {
        struct bar {
            std::uint16_t a{ 1 };
            std::uint32_t b{ 2 };
            std::uint16_t c{ 3 };
        };
        struct foo {
            std::uint64_t A{ 10 };
            std::array<std::byte, 3> arr{ std::byte{ 1 }, std::byte{ 2 }, std::byte{ 4 } };
            bar B{};
        };

        const auto X    = foo{};
        const auto Y    = foo{ .B = { .a = 2 } };
        const auto buff = byte_buff<2 * sizeof(foo)>{ X, Y };
        expect(buff.bytes().size() == 2 * sizeof(foo));

        const auto correct          = std::vector{ X, Y };
        const auto correct_as_bytes = std::as_bytes(std::span{ correct });

        expect(std::ranges::equal(buff.bytes(), correct_as_bytes));

        const auto incorrect          = std::vector{ X, X };
        const auto incorrect_as_bytes = std::as_bytes(std::span{ incorrect });
        expect(not std::ranges::equal(buff.bytes(), incorrect_as_bytes));
    };
};
