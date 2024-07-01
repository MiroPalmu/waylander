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

#include "byte_vec.hpp"
#include "gnu_utils/local_stream_socket.hpp"

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
}
