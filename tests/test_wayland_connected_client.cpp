#include <boost/ut.hpp> // import boost.ut;

/// These test do not try to use the possible Wayland compositor socket present on the system.

#include <algorithm>
#include <array>
#include <filesystem>
#include <future>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

#include "byte_vec.hpp"
#include "gnu_utils/local_stream_socket.hpp"
#include "wayland/message_parser.hpp"
#include "wayland/message_utils.hpp"
#include "wayland/protocol.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger::wl;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "connected_client object can be constructed from already connected socket"_test = [] {
        auto [client_sock, _] = ger::gnu::open_local_stream_socket_pair();
        expect(nothrow([&] { auto _ = connected_client{ std::move(client_sock) }; }));
    };

    wl_tag / "connected_client can reserve object id"_test = [] {
        auto [client_sock, _] = ger::gnu::open_local_stream_socket_pair();
        auto client           = connected_client{ std::move(client_sock) };
        const auto id1        = client.reserve_object_id<protocols::wl_display>();
        const auto id2        = client.reserve_object_id<protocols::wl_touch>();
        const auto id3        = client.reserve_object_id();

        expect(std::same_as<Wobject<protocols::wl_display>, std::remove_cvref_t<decltype(id1)>>);
        expect(std::same_as<Wobject<protocols::wl_touch>, std::remove_cvref_t<decltype(id2)>>);
        expect(std::same_as<Wobject<generic_object>, std::remove_cvref_t<decltype(id3)>>);
        expect(id1 == id1);
        expect(id1 != id2);
        expect(id1 != id3);
        expect(id2 == id2);
        expect(id2 != id3);
        expect(id3 == id3);
    };

    wl_tag / "connected_client can register requests"_test = [] {
        auto [client_sock, _] = ger::gnu::open_local_stream_socket_pair();
        auto client           = connected_client{ std::move(client_sock) };

        using namespace protocols;
        const auto registery = client.reserve_object_id<wl_registry>();
        const auto request   = wl_display::request::get_registry{ registery };

        expect(requires { client.register_request(global_display_object, request); });
    };

    wl_tag / "connected_client denies registeration for requests of different interface"_test = [] {
        using wl_display = protocols::wl_display;
        using wl_shm     = protocols::wl_shm;

        using wrong_Wobject   = Wobject<wl_shm>;
        using correct_Wobject = Wobject<wl_display>;

        using request = wl_display::request::get_registry;

        const auto register_request_is_valid_for_object = [](auto obj) {
            return requires(connected_client client, request req) {
                client.register_request(obj, req);
            };
        };

        expect(register_request_is_valid_for_object(correct_Wobject{}));
        expect(not register_request_is_valid_for_object(wrong_Wobject{}));
    };

    wl_tag / "connected_client can flush registered requests without file descriptors"_test = [] {
        auto [client_sock, server_sock] = ger::gnu::open_local_stream_socket_pair();
        auto client                     = connected_client{ std::move(client_sock) };

        using namespace protocols;
        const auto registery = client.reserve_object_id<wl_registry>();
        const auto request   = wl_display::request::get_registry{ registery };

#ifndef __clang__
        // This will crash LLVM 17 and 18 (annoyingly will break clangd), see:
        // https://github.com/llvm/llvm-project/issues/93821#issue-2325716041
        expect(static_message<decltype(request)>);
#endif

        // Send the same message multiple of times.
        constexpr auto msg_repeats_per_send = std::array{ 1uz, 2uz, 4uz, 8uz };
        constexpr auto amount_of_msg =
            std::ranges::fold_left(msg_repeats_per_send, 0uz, std::plus<std::size_t>{});

        constexpr auto msg_size =
            sizeof(message_header<wl_registry>) + message_payload_size(request);
        constexpr auto total_msg_size = msg_size * amount_of_msg;

        auto recv_data_fut = std::async(std::launch::async, [&] {
            auto buff = ger::sstd::byte_vec(total_msg_size);
            buff.resize(server_sock.read(buff));
            return buff;
        });

        for (const auto n : msg_repeats_per_send) {
            expect(not client.has_registered_requests());
            for (const auto _ : std::ranges::iota_view(0uz, n)) {
                client.register_request(global_display_object, request);
            }
            expect(client.has_registered_requests());

            client.flush_registered_requests();
            expect(not client.has_registered_requests());
        }

        const auto recv_data = recv_data_fut.get();
        expect(recv_data.size() == total_msg_size);

        auto parser  = message_parser{ recv_data };
        auto msg_gen = parser.message_generator();

        for (const auto recv_msg : msg_gen) {
            expect(recv_msg.object_id == global_display_object);
            expect(recv_msg.opcode == decltype(request)::opcode);
        }
    };

    wl_tag / "connected_client can be default constructed"_test = [] {
        const auto wayland_server_is_present = std::filesystem::is_socket(wayland_socket_path());
        expect(wayland_server_is_present) << "Working Wayland compositor is required for this test."
                                          << "Skipping rest of this test!\n";
        if (wayland_server_is_present) {
            // Stop executing rest of tests.
            return;
        }

        expect(nothrow([] { auto _ = connected_client{}; }));
    };
}
