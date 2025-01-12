// Copyright (C) 2024 Miro Palmu.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file.  If not, see <https://www.gnu.org/licenses/>.


#include <boost/ut.hpp> // import boost.ut;

/// These test do not try to use the possible Wayland compositor socket present on the system.

#include <algorithm>
#include <array>
#include <filesystem>
#include <future>
#include <ranges>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

#include "byte_vec.hpp"
#include "gnulander/fd_handle.hpp"
#include "gnulander/local_stream_socket.hpp"
#include "gnulander/memory_block.hpp"
#include "wayland/message_buffer.hpp"
#include "wayland/message_parser.hpp"
#include "wayland/message_utils.hpp"
#include "wayland/message_visitor.hpp"
#include "wayland/connected_client.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger::wl;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "connected_client object can be constructed from already connected socket"_test = [] {
        auto [client_sock, _] = gnulander::open_local_stream_socket_pair();
        expect(nothrow([&] { auto _ = connected_client{ std::move(client_sock) }; }));
    };

    wl_tag / "connected_client can reserve object id"_test = [&] {
        auto [client_sock, _] = gnulander::open_local_stream_socket_pair();
        auto client           = connected_client{ std::move(client_sock) };
        const auto id1        = client.reserve_object_id<protocols::wl_display>();
        const auto id2        = client.reserve_object_id<protocols::wl_touch>();
        const auto id3        = client.reserve_object_id();

        Wobject<protocols::wl_shell> id4;

        wl_tag / "reservation can be done using Wobject<...>::reserve"_test = [&] {
            const auto id4_copy = id4.reserve_id(client);
            expect(id4 == id4_copy);
        };

        const auto id5 = client.reserve_object_id<protocols::wl_shell>();

        wl_tag / "which types are correct"_test = [&] {
            const auto type_test = []<typename T, typename ExpectedInterface>(
                                       const T&,
                                       std::type_identity<ExpectedInterface>) -> bool {
                return std::same_as<T, Wobject<ExpectedInterface>>;
            };

            expect(type_test(id1, std::type_identity<protocols::wl_display>{}));
            expect(type_test(id2, std::type_identity<protocols::wl_touch>{}));
            expect(type_test(id3, std::type_identity<generic_object>{}));
            // Redundant test, as type of id4 is not deduced.
            expect(type_test(id4, std::type_identity<protocols::wl_shell>{}));
            expect(type_test(id5, std::type_identity<protocols::wl_shell>{}));
        };

        wl_tag / "which are monotonically increasing"_test = [&] {
            expect(global_display_object.value == 1);
            expect(id1.value == 2);
            expect(id2.value == 3);
            expect(id3.value == 4);
            expect(id4.value == 5);
            expect(id5.value == 6);
        };

        wl_tag / "which are comparable with eachother"_test = [&] {
            expect(id1 == id1);
            expect(id1 != id2);
            expect(id1 != id3);
            expect(id2 == id2);
            expect(id2 != id3);
            expect(id3 == id3);
            expect(id4 != id5);
        };
    };

    wl_tag / "connected_client can register requests"_test = [] {
        auto [client_sock, _] = gnulander::open_local_stream_socket_pair();
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
        auto [client_sock, server_sock] = gnulander::open_local_stream_socket_pair();
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

    wl_tag / "connected_client can flush registered requests with file descriptors"_test = [] {
        auto [client_sock, server_sock] = gnulander::open_local_stream_socket_pair();
        auto client                     = connected_client{ std::move(client_sock) };

        auto mem                = gnulander::memory_block{};
        constexpr auto mem_size = 10uz;
        mem.truncate(mem_size);
        for (auto& x : mem.map(mem_size)) { x = std::byte{ 10 }; }

        using wl_shm             = protocols::wl_shm;
        const auto shm_object_id = Wobject<wl_shm>{ 2u };
        const auto mem_ref       = gnulander::fd_ref{ mem };
        const auto mock_msg_with_fd =
            wl_shm::request::create_pool{ .id{ 42u }, .fd{ mem_ref }, .size{ 10 } };

        constexpr auto msg_size = sizeof(message_header<wl_shm>) + sizeof(mock_msg_with_fd.id)
                                  + sizeof(mock_msg_with_fd.size);
        static_assert(msg_size % 4 == 0);

        auto recd_fd_opt_future = std::async(std::launch::async, [&] {
            auto recv_buff = ger::sstd::byte_vec(msg_size);
            auto msg       = gnulander::local_socket_msg<1, 1>{ std::span{ recv_buff } };
            std::ignore    = server_sock.recv(msg);
            auto [fd_opt]  = msg.get_fd_handles();
            return fd_opt;
        });

        expect(not client.has_registered_requests());
        client.register_request(shm_object_id, mock_msg_with_fd);
        expect(client.has_registered_requests());

        client.flush_registered_requests();
        expect(not client.has_registered_requests());

        auto fd_opt = recd_fd_opt_future.get();
        expect(fatal(fd_opt.has_value()));

        auto fd = std::move(fd_opt).value();
        expect(std::ranges::all_of(fd.map(mem_size),
                                   [](const auto b) { return b == std::byte{ 10 }; }));
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

    wl_tag / "connected_client can recive events to message_parser"_test = [] {
        using shell_surface = protocols::wl_shell_surface;
        using event_t       = shell_surface::event::configure;
        using enum_t        = shell_surface::resize;
        const auto configure_event =
            event_t{ .edges = enum_t::Etop, .width{ 42u }, .height{ 13u } };

        // Send this many events from server.
        constexpr auto number_of_events = 14;

        auto [client_sock, server_sock] = gnulander::open_local_stream_socket_pair();
        auto client                     = connected_client{ std::move(client_sock) };

        const auto obj = client.reserve_object_id<shell_surface>();

        auto _ = std::async(std::launch::async, [&] {
            auto buff = message_buffer{};
            for (auto _ : std::ranges::iota_view(0, number_of_events)) {
                buff.append(obj, configure_event);
            }
            const auto data = buff.release_data();
            server_sock.write(data);
        });

        auto events_recved = 0;
        expect(events_recved != number_of_events);

        auto all_events_recved = [&] { return events_recved == number_of_events; };
        while (not all_events_recved()) {
            auto msg_parser = client.recv_events();
            auto msg_gen    = msg_parser.message_generator();

            for (const auto& msg : msg_gen) {
                expect(msg.opcode == event_t::opcode);
                expect(msg.object_id == obj);
                ++events_recved;
            }
        }
        expect(events_recved == number_of_events);
    };

    wl_tag / "connected_client can recive events which are sent one byte at the time"_test = [] {
        using shell_surface = protocols::wl_shell_surface;
        using event_t       = shell_surface::event::configure;
        using enum_t        = shell_surface::resize;
        const auto configure_event =
            event_t{ .edges = enum_t::Etop, .width{ 42u }, .height{ 13u } };

        // Send this many events from server.
        constexpr auto number_of_events = 14;

        auto [client_sock, server_sock] = gnulander::open_local_stream_socket_pair();
        auto client                     = connected_client{ std::move(client_sock) };

        const auto obj = client.reserve_object_id<shell_surface>();

        auto _ = std::async(std::launch::async, [&] {
            auto buff = message_buffer{};
            for (auto _ : std::ranges::iota_view(0, number_of_events)) {
                buff.append(obj, configure_event);
            }
            const auto data      = buff.release_data();
            const auto data_span = std::span{ data };
            for (const auto i : std::ranges::iota_view(0uz, data_span.size())) {
                server_sock.write(data_span.subspan(i, 1));
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1ms);
            }
        });

        auto events_recved = 0;
        expect(events_recved != number_of_events);

        auto all_events_recved = [&] { return events_recved == number_of_events; };
        while (not all_events_recved()) {
            auto msg_parser = client.recv_events();
            auto msg_gen    = msg_parser.message_generator();

            for (const auto& msg : msg_gen) {
                expect(msg.opcode == event_t::opcode);
                expect(msg.object_id == obj);
                ++events_recved;
            }
        }
        expect(events_recved == number_of_events);
    };

    wl_tag / "connected_client can visit messages until spesific one"_test = [&] {
        // Secenario setup:

        using wl_display   = protocols::wl_display;
        using get_registry = wl_display::request::get_registry;
        using wl_registry  = protocols::wl_registry;
        using bind         = wl_registry::request::bind;

        constexpr auto registry_obj = Wobject<wl_registry>{ 42 };
        constexpr auto display_obj  = Wobject<wl_display>{ 55 };

        // Assume that message_[parser|buffer] works and construct messages:
        //
        // 1. display_obj: get_registry{ 1 }
        // 2. registry_obj: bind{ 2, "abc", 42, 3 }
        // 3. display_obj: get_registry{ 4 }
        // 4. registry_obj: bind{ 5, "def", 43, 6 }
        // 5. global_display_object: get_registry{ 7 }
        // 6. display_obj: get_registry{ 8 }
        auto events = [&] {
            auto buff = message_buffer{};
            buff.append(display_obj, get_registry{ .registry{ 1 } });
            buff.append(registry_obj,
                        bind{ .name{ 2 },
                              .new_id_interface{ u8"abc" },
                              .new_id_interface_version{ 42 },
                              .id{ 3 } });
            buff.append(display_obj, get_registry{ .registry{ 4 } });
            buff.append(registry_obj,
                        bind{ .name{ 5 },
                              .new_id_interface{ u8"def" },
                              .new_id_interface_version{ 43 },
                              .id{ 6 } });
            buff.append(global_display_object, get_registry{ .registry{ 7 } });
            buff.append(display_obj, get_registry{ .registry{ 8 } });
            return buff.release_data();
        }();

        auto [client_sock, server_sock] = gnulander::open_local_stream_socket_pair();

        auto _ = std::async(std::launch::async, [&] { server_sock.write(events); });

        // Actual tests:

        auto client = connected_client{ std::move(client_sock) };

        auto request_count = 0uz;
        auto event_count   = 0uz;

        auto ov = message_overload_set{};

        wl_tag / "messages without overloads are skipped"_test = [&] {
            wl_tag / "the spesific message can be used"_test = [&] {
                client.recv_and_visit_events(ov).until<bind>(registry_obj, [](const bind& msg) {
                    expect(msg.name == 2);
                    expect(msg.new_id_interface == u8"abc");
                    expect(msg.new_id_interface_version == 42);
                    expect(msg.id == 3);
                });
                expect(request_count == 0);
                expect(event_count == 0);
            };
        };

        ov.add_overload<get_registry>(display_obj, [&](auto) { ++request_count; });
        ov.add_overload<bind>(registry_obj, [&](auto) { ++event_count; });

        client.recv_and_visit_events(ov).until<get_registry>(global_display_object);
        expect(request_count == 1);
        expect(event_count == 1);

        wl_tag / "the specified 'until message' takes priority over overloads"_test = [&] {
            client.recv_and_visit_events(ov).until<get_registry>(display_obj);
            expect(request_count == 1);
            expect(event_count == 1);
        };
    };
}
