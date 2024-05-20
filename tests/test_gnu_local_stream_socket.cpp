#include <boost/ut.hpp> // import boost.ut;

#include <algorithm>
#include <cstddef>
#include <future>
#include <latch>
#include <thread>
#include <vector>

#include "byte_vec.hpp"
#include "gnu_utils/local_stream_socket.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "gnu" } };

    tag("gnu") / "local_stream_socket pair can be constructed"_test = [] {
        [[maybe_unused]] auto [_, _] = ger::gnu::open_local_stream_socket_pair();
        expect(true);
    };

    tag("gnu") / "local_stream_socket is writable and readable"_test = [] {
        expect(requires(ger::gnu::local_stream_socket socket, std::span<const std::byte> s) {
            socket.write(s);
        });
        expect(requires(ger::gnu::local_stream_socket socket, std::span<std::byte> s) {
            socket.read(s);
        });
    };

    tag("gnu") / "local_stream_socket ping pong"_test = [] {
        //       A                         B
        //       │                         │
        // expect(ping != recv_buff_B)     │
        // expect(pong != recv_buff_A)     │
        //       │                         │
        //       ├──│ping -> recv_buff_B)──┤
        //       │                         │
        //       ├──(recv_buff_A <- pong│──┘
        //       │
        // expect(ping == recv_buff_B)
        // expect(pong == recv_buff_A)
        //
        const auto ping = std::vector<std::byte>{ std::byte{ 'p' },
                                                  std::byte{ 'i' },
                                                  std::byte{ 'n' },
                                                  std::byte{ 'g' } };

        const auto pong = std::vector<std::byte>{ std::byte{ 'p' },
                                                  std::byte{ 'o' },
                                                  std::byte{ 'n' },
                                                  std::byte{ 'g' },
                                                  std::byte{ '!' } };

        auto recv_buff_A = std::vector<std::byte>{ std::byte{ 'b' },
                                                   std::byte{ 'u' },
                                                   std::byte{ 'f' },
                                                   std::byte{ 'f' },
                                                   std::byte{ '1' } };

        auto recv_buff_B = std::vector<std::byte>{ std::byte{ 'b' },
                                                   std::byte{ 'u' },
                                                   std::byte{ 'f' },
                                                   std::byte{ 'f' } };

        expect(ping != recv_buff_B);
        expect(pong != recv_buff_A);
        expect(ping.size() == recv_buff_B.size());
        expect(pong.size() == recv_buff_A.size());

        auto [A, B] = ger::gnu::open_local_stream_socket_pair();

        auto ping_done       = std::latch{ 2 };
        auto read_bytes_ping = std::async(std::launch::async, [&] {
            const auto read_bytes = B.read(recv_buff_B);
            ping_done.arrive_and_wait();
            B.write(pong);
            return read_bytes;
        });

        A.write(ping);
        ping_done.arrive_and_wait();
        const auto read_bytes_pong = A.read(recv_buff_A);

        expect(read_bytes_ping.get() == ping.size());
        expect(read_bytes_pong == pong.size());
        expect(ping == recv_buff_B);
        expect(pong == recv_buff_A);
    };

    tag("gnu") / "read_some does not block when trying to read more than is available"_test = [] {
        const auto ping = sstd::byte_vec{ std::byte{ 'p' },
                                          std::byte{ 'i' },
                                          std::byte{ 'n' },
                                          std::byte{ 'g' } };

        auto [A, B] = ger::gnu::open_local_stream_socket_pair();
        auto _      = std::jthread{ [&] { B.write(ping); } };

        auto recv_buff = sstd::byte_vec{};
        recv_buff.resize(2 * ping.size());
        const auto read_bytes = A.read_some(recv_buff);
        expect(read_bytes == ping.size());
        expect(std::ranges::equal(ping, std::span(recv_buff.cbegin(), read_bytes)));
    };
}
