#include <config.h>

#include <boost/ut.hpp> // import boost.ut;

// These are for make_named_socket.
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <format>
#include <future>
#include <latch>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <text_encoding>
#include <thread>
#include <vector>
#include <version>

#include "byte_vec.hpp"
#include "gnu_utils/local_stream_socket.hpp"
#include "gnu_utils/memory_block.hpp"

namespace testing {

/// Testing utility for creating socket binded to local-namespace.
///
/// From GNU libc manual 16.5.3 Example of Local-Namespace Socket.
int make_named_socket(const char* filename) {
    struct sockaddr_un name;
    int sock;
    size_t size;

    /* Create the socket. */
    sock = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Bind a name to the socket. */
    name.sun_family = AF_LOCAL;
    strncpy(name.sun_path, filename, sizeof(name.sun_path));
    name.sun_path[sizeof(name.sun_path) - 1] = '\0';

    /* The size of the address is
     the offset of the start of the filename,
     plus its length (not including the terminating null byte).
     Alternatively you can just do:
     size = SUN_LEN (&name);
    */
    size = (offsetof(struct sockaddr_un, sun_path) + strlen(name.sun_path));

    if (bind(sock, (struct sockaddr*)&name, size) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    return sock;
}
} // namespace testing

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

    tag("gnu") / "socket from open_local_stream_socket_connected_to is usable"_test = [&] {
        auto tmpdir_cstr        = std::getenv("TMPDIR");
        const auto temp_dir_opt = [&]() -> std::optional<std::string> {
            if (tmpdir_cstr == nullptr) {
                if (std::filesystem::is_directory("/tmp")) { return "/tmp"; }
                return {};
            }
            return tmpdir_cstr;
        }();

        expect(temp_dir_opt.has_value())
            << "This test requires $TMPDIR to be set or /tmp to be directory.\n"
            << "Skipping rest of this test!\n";
        if (not temp_dir_opt.has_value()) {
            // Stop executing rest of tests.
            return;
        }

        const auto temp_dir = temp_dir_opt.value();

        // Outline of this test:
        //
        // 1) create new testing directory testing_dir with mkdtemp
        // 2) open server socket in that directory with testing::make_named_socket
        // 3) open local_stream_socket connected to that socket and accept it on another thread
        // 4) on that anohter thread send msg and recv it on "this"/"main" thread
        // 5) clean up:
        //     - close fd created by ::accept(testing_socket, ...) on the another thread
        //     - close testing_socket fd
        //     - recursivly delete the testing dir contents and the directory itself

        auto testing_dir          = temp_dir + std::string{ "/guilander_test_dir-XXXXXX" };
        const auto mkdtemp_failed = (nullptr == mkdtemp(testing_dir.data()));

        if (mkdtemp_failed) {
            expect(false) << std::format("mkdtemp({}) failed", testing_dir);
            return;
        }

        const auto testing_socket_name = testing_dir + "/test_socket";

        expect(not std::filesystem::is_socket(testing_socket_name))
            << std::format("socket '{}' is already present", testing_socket_name);

        const auto testing_socket = testing::make_named_socket(testing_socket_name.c_str());
        if (-1 == listen(testing_socket, 1)) { sstd::throw_generic_system_error(); }
        expect(std::filesystem::is_socket(testing_socket_name));

        const auto msg =
            std::vector{ std::byte{ 'p' }, std::byte{ 'i' }, std::byte{ 'n' }, std::byte{ 'g' } };

        auto another_thread = std::async(std::launch::async, [&] {
            const auto server_socket = ::accept(testing_socket, nullptr, nullptr);
            if (-1 == server_socket) { sstd::throw_generic_system_error(); }
            if (std::ranges::ssize(msg) != ::write(server_socket, msg.data(), msg.size())) {
                sstd::throw_generic_system_error();
            }
            if (-1 == ::close(server_socket)) { sstd::throw_generic_system_error(); }
        });

        auto client_socket    = gnu::open_local_stream_socket_connected_to(testing_socket_name);
        auto recv_buff        = std::vector<std::byte>(msg.size());
        const auto recv_bytes = client_socket.read(recv_buff);
        expect(msg.size() == recv_bytes);
        expect(msg == recv_buff);

        // clean up:
        another_thread.wait();
        if (-1 == ::close(testing_socket)) { sstd::throw_generic_system_error(); }
        std::filesystem::remove_all(testing_dir);

        expect(not std::filesystem::is_socket(testing_socket_name))
            << std::format("Failed to clean up testing socket: '{}'", testing_socket_name);
        expect(not std::filesystem::is_directory(testing_dir))
            << std::format("Failed to clean up testing directory: '{}'", testing_dir);
    };

    tag("gnu")
        / "open_local_stream_socket_connected_to throws if server socket does not exits"_test =
        [&] {
            const auto random_path = std::filesystem::path{ "/foo/bar/no/socket" };
            expect(not std::filesystem::is_socket(random_path));
            expect(throws<std::system_error>(
                [&] { std::ignore = gnu::open_local_stream_socket_connected_to(random_path); }));
        };

    tag("gnu")
        / "local_stream_socket can [send|recev] one byte local_socket_message<1, 0>"_test = [] {
        auto [alpha, beta] = ger::gnu::open_local_stream_socket_pair();

        auto _ = std::jthread{ [&] {
            auto recv_buff = std::array{ std::byte{ 10 } };

            auto msg = ger::gnu::local_socket_msg<1, 0>{ std::span{ recv_buff } };
            std::ignore = beta.recv(msg);
            if (recv_buff[0] == std::byte{ 42 }) { recv_buff[0] = std::byte{ 24 }; }
            std::ignore = beta.send(msg);
        } };

        auto recv_buff = std::array{ std::byte{ 42 } };
        auto msg       = ger::gnu::local_socket_msg<1, 0>{ std::span{ recv_buff } };

        const auto send_num = alpha.send(msg);
        const auto recv_num = alpha.recv(msg);

        expect(send_num == 1);
        expect(recv_num == 1);

        const auto this_thread_recv_ok =
            recv_buff[0] == std::byte{ 24 } or recv_buff[0] == std::byte{ 10 };

        const auto other_thread_recv_ok = recv_buff[0] == std::byte{ 24 };

        expect(this_thread_recv_ok);
        expect(other_thread_recv_ok);
    };

    tag("gnu") / "local_stream_socket can send|recev a local_socket_message<1, 1>"_test = [] {
        // Scenario setup:

        auto [alpha, beta] = ger::gnu::open_local_stream_socket_pair();

        // Alpha sends filedescriptor to beta.
        // If beta receives it, bit-shift every byte by one.

        enum recv_fd_cases { no_fd, wrong_data, shift_done };
        auto state_promise = std::promise<recv_fd_cases>{};
        auto state         = state_promise.get_future();

        // Alpha setup:

        auto some_byte = std::array{ std::byte{ 42 } };

        constexpr auto mem_size = 10uz;
        auto mem                = gnu::memory_block{};
        mem.truncate(mem_size);
        const auto mapped_mem = mem.map(mem_size);
        std::ranges::fill(mapped_mem, std::byte{ 10 });

        // Beta setup:

        auto _ = std::jthread{ [&] {
            auto recv_buff = std::array<std::byte, 1>{};

            auto msg = gnu::local_socket_msg<1, 1>{ std::span{ recv_buff } };
            std::ignore = beta.recv(msg);

            if (recv_buff[0] != std::byte{ 42 }) {
                state_promise.set_value(wrong_data);
                return;
            }

            auto [fd_opt] = msg.get_fd_handles();
            if (not fd_opt.has_value()) {
                state_promise.set_value(no_fd);
                return;
            }

            auto fd = std::move(fd_opt).value();
            for (auto& x : fd.map(mem_size)) { x <<= 1; }
            fd.close();
            state_promise.set_value(shift_done);
        } };

        // Actual tests:

        auto msg = gnu::local_socket_msg<1, 1>{ gnu::fd_ref{ mem }, std::span{ some_byte } };

        expect(alpha.send(msg) == 1);
        expect(fatal(state.get() == shift_done));

        expect(std::ranges::all_of(mapped_mem,
                                   [](const auto x) { return x == (std::byte{ 10 } << 1); }));
    };

    tag("gnu") / "local_stream_socket can send|recev a local_socket_message<1, 2>"_test = [] {
        // Scenario setup:

        auto [alpha, beta] = ger::gnu::open_local_stream_socket_pair();

        // Alpha will send a one byte and two file descriptors to beta.
        // The one byte is assumed to be received, but one can not assume the amount
        // of received file descriptors.
        //
        // Beta will act accordingly based on file decriptors received:
        //
        // 0: Abort.
        // A: Bit-shift every byte by one of the file.
        // B: Abort.
        // AB: Bit-shift every byte by two
        // BA: Abort.

        enum recv_fd_cases { no_neither_fd, one_fd_A, one_fd_B, both_fd, switched_fd };

        auto state_promise = std::promise<recv_fd_cases>{};
        auto state         = state_promise.get_future();

        // Alpha setup:

        std::byte some_byte[1]{ std::byte{ 'z' } };

        constexpr auto mem_size = 10uz;

        auto mem_A = gnu::memory_block{};
        mem_A.truncate(mem_size);
        const auto mapped_mem_A = mem_A.map(mem_size);
        std::ranges::fill(mapped_mem_A, std::byte{ 10 });

        auto mem_B = gnu::memory_block{};
        mem_B.truncate(mem_size);
        const auto mapped_mem_B = mem_B.map(mem_size);
        std::ranges::fill(mapped_mem_B, std::byte{ 20 });

        // Beta setup:

        auto _ = std::jthread{ [&] {
            std::byte recv_buff[1];
            auto msg = gnu::local_socket_msg<1, 2>{ std::span{ recv_buff } };

            std::ignore = beta.recv(msg);

            auto [fdA_opt, fdB_opt] = msg.get_fd_handles();
            const auto no_fdA = not fdA_opt.has_value();
            const auto no_fdB = not fdB_opt.has_value();

            if (no_fdA and no_fdB) {
                state_promise.set_value(no_neither_fd);
                return;
            }

            if (no_fdA) {
                state_promise.set_value(one_fd_B);
                fdB_opt.value().close();
                return;
            } else if (no_fdB) {
                auto the_one = std::move(fdA_opt).value();
                for (auto& x : the_one.map(mem_size)) { x <<= 1; }
                state_promise.set_value(one_fd_A);
                return;
            }

            auto fdA = std::move(fdA_opt).value();
            auto fdB = std::move(fdB_opt).value();

            const auto AB_switched = std::ranges::all_of(fdA.map(mem_size), [](const auto x) {
                return x == std::byte{ 20 };
            });

            if (AB_switched) {
                state_promise.set_value(switched_fd);
            } else {
                for (auto& x : fdA.map(mem_size)) { x <<= 2; }
                for (auto& x : fdB.map(mem_size)) { x <<= 2; }
            }

            fdA.close();
            fdB.close();
            state_promise.set_value(both_fd);
        } };

        // Actual tests:

        auto msg = gnu::local_socket_msg<1, 2>{ std::span{ some_byte },
                                                gnu::fd_ref{ mem_A },
                                                gnu::fd_ref{ mem_B } };
        expect(alpha.send(msg) == 1);

        const auto what_scenario = state.get();
        expect(fatal(what_scenario != no_neither_fd));
        expect(fatal(what_scenario != one_fd_B));
        expect(fatal(what_scenario != switched_fd));

        if (what_scenario == one_fd_A) {
            expect(std::ranges::all_of(mapped_mem_A,
                                       [](const auto x) { return x == (std::byte{ 10 } << 1); }));
        } else if (what_scenario == both_fd) {
            expect(std::ranges::all_of(mapped_mem_A,
                                       [](const auto x) { return x == (std::byte{ 10 } << 2); }));
            expect(std::ranges::all_of(mapped_mem_B,
                                       [](const auto x) { return x == (std::byte{ 20 } << 2); }));

        } else {
            expect(false) << "Should not happen. Corrupted state?";
        }
    };

    tag("gnu") / "local_socket_msg<n, 0> for n > 1 can be received by *<1, 0>"_test = [] {
        // Scenario setup:

        auto [alpha, beta] = ger::gnu::open_local_stream_socket_pair();

        constexpr auto wrong_data_indicator = 1000000uz;
        /// If beta receives wrong data, it will add wrong_data_indicator to amount of recd data.
        auto beta_recd_bytes_promise = std::promise<std::size_t>{};
        auto beta_recd_bytes         = beta_recd_bytes_promise.get_future();

        // Alpha setup:

        std::array<std::byte, 1> data_A{ std::byte{ 1 } };
        std::array<std::byte, 2> data_B{ std::byte{ 2 }, std::byte{ 3 } };
        auto data_C = std::vector{ std::byte{ 4 }, std::byte{ 5 }, std::byte{ 6 } };
        auto data_D = std::vector{ std::byte{ 7 }, std::byte{ 8 }, std::byte{ 9 } };

        const auto total_data_size = data_A.size() + data_B.size() + data_C.size() + data_D.size();
        const auto correct_data =
            std::views::iota(1)
            | std::views::transform([](const auto b) { return static_cast<std::byte>(b); })
            | std::views::take(total_data_size);

#ifdef __cpp_lib_ranges_concat
#    if __cpp_lib_ranges_concat >= 202403L
        expect(fatal(
            std::ranges::equal(correct_data, std::views::concat(data_A, data_B, data_C, data_D))));
#    endif
#endif

        // Beta setup:

        auto _ = std::jthread{ [&] {
            auto recv_buff = std::vector<std::byte>(total_data_size);
            auto msg = gnu::local_socket_msg<1, 0>{ std::span{ recv_buff } };

            const auto recd_bytes = beta.recv(msg);
            const auto recd_ok =
                std::ranges::equal(recv_buff, correct_data | std::views::take(recd_bytes));
            beta_recd_bytes_promise.set_value(recd_bytes + (recd_ok ? 0uz : wrong_data_indicator));
        } };

        // Actual tests:

        auto msg = gnu::local_socket_msg<4>{ std::span{ data_A },
                                             std::span{ data_B },
                                             std::span{ data_C },
                                             std::span{ data_D } };

        // Note that local_stream_socket::send only promises to deliver one byte.
        const auto send_bytes = alpha.send(msg);
        expect(send_bytes >= 1);
        expect(send_bytes <= total_data_size);
        const auto recd_bytes = beta_recd_bytes.get();
        expect(recd_bytes < wrong_data_indicator)
            << std::format("Beta received {} bytes of wrong data.",
                           recd_bytes - wrong_data_indicator);
        expect(recd_bytes == send_bytes);
    };

    tag("gnu")
        / "local_socket_message can receive less file descriptors than it expects"_test = [] {
        // Scenario setup:

        auto [alpha, beta] = ger::gnu::open_local_stream_socket_pair();

        // Alpha will send a one byte and two file descriptors to beta,
        // which expect to receive 4 file descriptors.
        // The one byte is assumed to be received, but one can not assume the amount
        // of received file descriptors.
        //
        // Beta will act accordingly based on file decriptors received:
        //
        // 0: Abort.
        // A: Bit-shift every byte by one of the file.
        // B: Abort.
        // AB: Bit-shift every byte by two
        // BA: Abort.
        // more than two: abort ??

        enum recv_fd_cases {
            no_neither_fd,
            one_fd_A,
            one_fd_B,
            both_fd,
            switched_fd,
            more_than_two
        };

        auto state_promise = std::promise<recv_fd_cases>{};
        auto state         = state_promise.get_future();

        // Alpha setup:

        std::byte some_byte[1]{ std::byte{ 'z' } };

        constexpr auto mem_size = 10uz;

        auto mem_A = gnu::memory_block{};
        mem_A.truncate(mem_size);
        const auto mapped_mem_A = mem_A.map(mem_size);
        std::ranges::fill(mapped_mem_A, std::byte{ 10 });

        auto mem_B = gnu::memory_block{};
        mem_B.truncate(mem_size);
        const auto mapped_mem_B = mem_B.map(mem_size);
        std::ranges::fill(mapped_mem_B, std::byte{ 20 });

        // Beta setup:

        auto _ = std::jthread{ [&] {
            std::byte recv_buff[1];
            auto msg = gnu::local_socket_msg<1, 4>{ std::span{ recv_buff } };

            std::ignore = beta.recv(msg);

            auto [fdA_opt, fdB_opt, fdC_opt, fdD_opt] = msg.get_fd_handles();

            if (fdC_opt or fdD_opt) {
                state_promise.set_value(more_than_two);
                return;
            }

            const auto no_fdA = not fdA_opt.has_value();
            const auto no_fdB = not fdB_opt.has_value();

            if (no_fdA and no_fdB) {
                state_promise.set_value(no_neither_fd);
                return;
            }

            if (no_fdA) {
                state_promise.set_value(one_fd_B);
                fdB_opt.value().close();
                return;
            } else if (no_fdB) {
                auto the_one = std::move(fdA_opt).value();
                for (auto& x : the_one.map(mem_size)) { x <<= 1; }
                state_promise.set_value(one_fd_A);
                return;
            }

            auto fdA = std::move(fdA_opt).value();
            auto fdB = std::move(fdB_opt).value();

            const auto AB_switched = std::ranges::all_of(fdA.map(mem_size), [](const auto x) {
                return x == std::byte{ 20 };
            });

            if (AB_switched) {
                state_promise.set_value(switched_fd);
            } else {
                for (auto& x : fdA.map(mem_size)) { x <<= 2; }
                for (auto& x : fdB.map(mem_size)) { x <<= 2; }
            }

            fdA.close();
            fdB.close();
            state_promise.set_value(both_fd);
        } };

        // Actual tests:

        auto msg = gnu::local_socket_msg<1, 2>{ std::span{ some_byte },
                                                gnu::fd_ref{ mem_A },
                                                gnu::fd_ref{ mem_B } };
        expect(alpha.send(msg) == 1);

        const auto what_scenario = state.get();
        expect(fatal(what_scenario != no_neither_fd));
        expect(fatal(what_scenario != one_fd_B));
        expect(fatal(what_scenario != switched_fd));
        expect(fatal(what_scenario != more_than_two));

        if (what_scenario == one_fd_A) {
            expect(std::ranges::all_of(mapped_mem_A,
                                       [](const auto x) { return x == (std::byte{ 10 } << 1); }));
        } else if (what_scenario == both_fd) {
            expect(std::ranges::all_of(mapped_mem_A,
                                       [](const auto x) { return x == (std::byte{ 10 } << 2); }));
            expect(std::ranges::all_of(mapped_mem_B,
                                       [](const auto x) { return x == (std::byte{ 20 } << 2); }));

        } else {
            expect(false) << "Should not happen. Corrupted state?";
        }
    };

    tag("gnu") / "local_socket_msg::get_fd_handles throws when some fd is truncated"_test = [] {
        // Scenario setup:

        auto [alpha, beta] = ger::gnu::open_local_stream_socket_pair();

        // Beta will send a one byte and two file descriptors to alpha,
        // which expect to receive only one file descriptor.
        //
        // However, due to arcane implementation details, even if alpha expects only one
        // there might be room for more. Behaviour for this is implemented as emulation
        // of truncation so it should just close all received file descriptors and throw.

        // Beta setup:

        constexpr auto mem_size = 10uz;
        auto _                  = std::jthread{ [&] {
            std::byte some_byte[1]{ std::byte{ 'z' } };

            auto mem_A          = gnu::memory_block{};
            mem_A.truncate(mem_size);
            const auto mapped_mem_A = mem_A.map(mem_size);
            std::ranges::fill(mapped_mem_A, std::byte{ 10 });

            auto mem_B          = gnu::memory_block{};
            mem_B.truncate(mem_size);
            const auto mapped_mem_B = mem_B.map(mem_size);
            std::ranges::fill(mapped_mem_B, std::byte{ 20 });

            auto msg            = gnu::local_socket_msg<1, 2>{ std::span{ some_byte },
                                                               gnu::fd_ref{ mem_A },
                                                               gnu::fd_ref{ mem_B } };
            std::ignore         = beta.send(msg);
        } };

        // Actual tests:

        std::byte recv_buff[1];
        auto msg = gnu::local_socket_msg<1, 1>{ std::span{ recv_buff } };

        std::ignore = alpha.recv(msg);
        expect(throws<std::domain_error>([&] { std::ignore = msg.get_fd_handles(); }));
    };
}
