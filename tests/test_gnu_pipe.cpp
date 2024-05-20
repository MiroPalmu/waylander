#include <boost/ut.hpp> // import boost.ut;

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <thread>
#include <vector>

#include "gnu_utils/pipe.hpp"

auto make_mock_buff(const std::size_t size) -> std::vector<std::byte> {
    namespace rv = std::views;
    return rv::iota(0uz, size) | rv::transform([](const std::size_t x) -> std::byte {
               return static_cast<std::byte>(x % 64uz);
           })
           | std::ranges::to<std::vector>();
}

int main() {
    using namespace boost::ut;
    cfg<override> = { .tag = { "gnu" } };

    tag("gnu") / "[in|out]_pipe can be constructed"_test = [] {
        [[maybe_unused]] auto [/* in */ _, /* out */ _] = ger::gnu::open_pipe();
        expect(true);
    };

    /// Has to be in generic lambda to make requires expressions not ill-formed.
    tag("gnu") / "[in|out]_pipe is not [writable|readable]"_test = []<typename> {
        const auto in_pipe_is_writable =
            requires(ger::gnu::in_pipe in, std::span<const std::byte> s) { in.write(s); };
        const auto out_pipe_is_readable =
            requires(ger::gnu::out_pipe out, std::span<std::byte> s) { out.read(s); };

        expect(not in_pipe_is_writable);
        expect(not out_pipe_is_readable);
    } | std::tuple<int>{};

    tag("gnu") / "[in|out]_pipe has get_PIPE_BUF"_test = [] {
        expect(requires(ger::gnu::in_pipe in) {
            { in.get_PIPE_BUF() } -> std::common_with<unsigned long>;
        });
        expect(requires(ger::gnu::out_pipe out) {
            { out.get_PIPE_BUF() } -> std::common_with<unsigned long>;
        });
    };

    tag("gnu") / "[in|out]_pipe can be [read|written] twice"_test = [] {
        auto [in, out]          = ger::gnu::open_pipe();
        const auto data_to_send = std::vector<std::byte>{ std::byte{ 7 },
                                                          std::byte{ 42 },
                                                          std::byte{ 8 },
                                                          std::byte{ 43 } };

        // Write:
        const auto ws1 = std::span<const std::byte>{ data_to_send };
        const auto ws2 = std::span<const std::byte>{ std::next(data_to_send.begin()),
                                                     std::prev(data_to_send.end()) };
        const auto _   = std::jthread([&] {
            out.write(ws1);
            out.write(ws2);
        });

        // Read:
        auto read_buff = std::vector<std::byte>(data_to_send.size());

        // Case 1:
        const auto rs1 = std::span<std::byte>{ read_buff };
        expect(data_to_send != read_buff);

        const auto bytes_read1 = in.read(rs1);
        expect(bytes_read1 == rs1.size());
        expect(data_to_send == read_buff);

        // Case 2:
        const auto rs2         = std::span<std::byte>{ read_buff.begin(), 2 };
        const auto bytes_read2 = in.read(rs2);

        expect(bytes_read2 == rs2.size());

        // These are not same anymore.
        expect(data_to_send != read_buff);
        // As:
        expect(read_buff[0] == std::byte{ 42 });
        expect(read_buff[1] == std::byte{ 8 });
    };

    tag("gnu") / "[in|out]_pipe can be [read|written] N bytes twice, s.t. N >> PIPE_BUF"_test = [] {
        auto [in, out] = ger::gnu::open_pipe();

        constexpr auto m{ 100uz }; // >> 1
        const auto N            = m * in.get_PIPE_BUF();
        const auto data_to_send = make_mock_buff(N + 2);

        // Write:
        const auto ws1 = std::span<const std::byte>{ data_to_send };
        const auto ws2 = std::span<const std::byte>{ std::next(data_to_send.begin()),
                                                     std::prev(data_to_send.end()) };
        const auto _   = std::jthread([&] {
            out.write(ws1);
            out.write(ws2);
        });

        // Read:
        auto read_buff = std::vector<std::byte>(data_to_send.size());

        // Case 1:
        const auto rs1 = std::span<std::byte>{ read_buff };
        expect(not std::ranges::equal(rs1, ws1));
        expect(in.read(rs1) == rs1.size());
        expect(std::ranges::equal(rs1, ws1));

        // Case 2:
        const auto rs2 = std::span<std::byte>{ read_buff.begin(), N };
        expect(not std::ranges::equal(rs2, ws2));
        expect(in.read(rs2) == rs2.size());
        expect(std::ranges::equal(rs2, ws2));
    };
}
