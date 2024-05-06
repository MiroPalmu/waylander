#include <boost/ut.hpp> // import boost.ut;

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <format>
#include <memory>
#include <new>
#include <ranges>
#include <vector>

#include "construct_allocator_adapter.hpp"

struct ctor_arg_counter {
    std::size_t count;
    template<typename... Arg>
    constexpr ctor_arg_counter(Arg...) : count{ sizeof...(Arg) } {}
};

// Deifine CAA := construct_allocator_adapte
template<typename ConstructImpl, typename T>
using CAA = ger::sstd::allocator::construct_allocator_adapter<ConstructImpl, T>;

int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "sstd" } };

    tag("sstd") / "CAA satisfies allocator_adapter concept"_test = [] {
        static_assert(
            sstd::allocator::allocator_adapter<CAA<sstd::allocator::constant_construct<0>, int>>);
        static_assert(
            sstd::allocator::allocator_adapter<CAA<sstd::allocator::default_init_construct, int>>);
        static_assert(sstd::allocator::allocator_adapter<
                      CAA<sstd::allocator::constant_construct<std::byte{}>, std::byte>>);
        static_assert(sstd::allocator::allocator_adapter<
                      CAA<sstd::allocator::default_init_construct, std::byte>>);
        expect(true);
    };

    tag("sstd") / "allocator_traits<CAA>::construct uses CAA::construct"_test = [] {
        auto counters = std::array<ctor_arg_counter, 3>{};
        std::ranges::for_each(counters, [](const auto& c) { expect(c.count == 0); });

        constexpr auto C = ctor_arg_counter(1, 2, 3);
        expect(C.count == 3);
        using A        = CAA<sstd::allocator::constant_construct<C>, ctor_arg_counter>;
        using A_traits = std::allocator_traits<A>;
        auto alloc     = A{};

        // Following constructions are ok, as:
        static_assert(std::is_trivially_destructible_v<ctor_arg_counter>);
        // so the storage in counters can be reused.
        A_traits::construct(alloc, &counters[0]);
        A_traits::construct(alloc, &counters[1], 1);
        A_traits::construct(alloc, &counters[2], 1, 2);

        std::ranges::for_each(counters, [](const auto& c) { expect(c.count == 3); });
    };

    tag("sstd") / "CAA with constant_construct can be used with std::vector"_test = [] {
        constexpr auto C = 42;
        using A          = CAA<sstd::allocator::constant_construct<C>, int>;
        auto vec         = std::vector<int, A>(10);
        expect(vec.size() == 10);
        expect(std::ranges::all_of(vec, [](const auto& x) { return x == C; }));
        vec.clear();
        expect(vec.empty());

        // emplace_back uses std::allocator_traits<A>::construct(...).
        vec.emplace_back();
        vec.emplace_back(1);
        vec.emplace_back(1, 2);
        vec.emplace_back(1, 2, 3);

        expect(std::ranges::all_of(vec, [](const auto& x) { return x == C; }));

        vec.clear();
        expect(vec.empty());

        // Should append default-inserted values.
        vec.resize(10);
        expect(std::ranges::all_of(vec, [](const auto& x) { return x == C; }));
    };

    tag("sstd") / "default-init placement new of std::byte is noop"_test = [] {
        constexpr auto B = std::byte{ 42 };
        auto b           = B;
        expect(b == B);
        // Must use qualified ::new and cast ptr to void*, to call true placement-new.
        // https://en.cppreference.com/w/cpp/memory/new/operator_new#Notes
        auto default_init_new_b = ::new (static_cast<void*>(&b)) std::byte;
        // Returned pointer has to be used, as reusing the storage of b ends its lifetime.
        expect(*default_init_new_b == B);

        auto value_init_new_b = ::new (static_cast<void*>(&b)) std::byte{};
        // Returned pointer has to be used, similarly to above.
        expect(*value_init_new_b == std::byte{});
    };

    tag("sstd")
        / "default_init_construct::construct default-inits when no arguments are given"_test = [] {
        using A        = CAA<sstd::allocator::default_init_construct, std::byte>;
        auto alloc     = A{};
        using A_traits = std::allocator_traits<A>;

        auto b = std::byte{ 1 };
        expect(b == std::byte{ 1 });

        // Value init with arguments.
        A_traits::construct(alloc, &b, std::byte{ 2 });

        // As above will call palcement-new with address of b, it should end lifetime of b.
        // However it constructs a new std::byte at its place, so to make accessing it not UB,
        // std::launder has to be used.
        // expect(*std::launder(&b) == std::byte{ 2 });

        // std::launder is not actually needed as std::byte is transparently replaceable by
        // another std::byte, so all references to the old object will automatically refer the new one.
        expect(b == std::byte{ 2 });

        // Should default-initialize std::byte at &b which does nothing.
        A_traits::construct(alloc, &b);

        // Possible UB depending on CWG1997 [1], as by above b now references the new object
        // and CWG1997 is not resolved as of 2024-05-06.
        // [1] https://cplusplus.github.io/CWG/issues/1997.html
        expect(b == std::byte{ 2 });

        // To make sure there is not lingering UB after CWG1997 is resolved,
        // make this test fail after check_interval has passed after date of_checking_CHW1997.
        using namespace std::chrono_literals;
        using namespace std::chrono;
        constexpr auto check_interval = months{ 4 };
        constexpr auto date_of_checking_CWH1997 =
            year_month_day(2024y, std::chrono::May, 6d); // Update me.
        const auto date_of_next_CWH1997_check = date_of_checking_CWH1997 + check_interval;
        const auto date_now                   = year_month_day{ floor<days>(system_clock::now()) };
        const auto CHW1997_should_be_checked  = date_now > date_of_next_CWH1997_check;
        expect(not CHW1997_should_be_checked)
            << std::format("Date of next check of CWG1997 is {} and today is {}!",
                           date_of_next_CWH1997_check,
                           date_now);
    };

    tag("sstd") / "by default std::vector::resize zeroes out std::byte (value-init)"_test = [] {
        auto vec = std::vector<std::byte>(10, std::byte{ 10 });
        std::ranges::for_each(vec, [](const auto& b) { expect(b == std::byte{ 10 }); });
        vec.resize(5);
        vec.resize(10);
        for (auto i = 0uz; i < vec.size(); ++i) {
            const auto x = i < 5 ? std::byte{ 10 } : std::byte{ 0 };
            expect(vec[i] == x);
        }
    };

    tag("sstd")
        / "std::vector::resize with default_init_construct does not zeroes out std::byte (default-init)"_test =
        [] {
            using A = sstd::allocator::
                construct_allocator_adapter<sstd::allocator::default_init_construct, std::byte>;

            auto vec = std::vector<std::byte, A>(10, std::byte{ 10 });
            std::ranges::for_each(vec, [](const auto& b) { expect(b == std::byte{ 10 }); });

            // As std::byte is trivially destructible type, assume that the dtors are noops.
            vec.resize(5);
            vec.resize(10);
            // At this point the values of vec at indecies [5, 10) are indeterminate,
            // so the following check is UB. If this starts failing then so be it.
            std::ranges::for_each(vec, [](const auto& b) { expect(b == std::byte{ 10 }); });
        };
}
