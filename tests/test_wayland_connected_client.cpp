#include <boost/ut.hpp> // import boost.ut;

#include <type_traits>
#include <utility>

#include "gnu_utils/local_stream_socket.hpp"
#include "wayland/protocol.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger::wl;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "connected_client object can be constructed without arguments"_test = [] {
        expect(nothrow([] { auto _ = connected_client(); }));
    };

    wl_tag / "connected_client object can be constructed from already connected socket"_test = [] {
        auto [socket, _] = ger::gnu::open_local_stream_socket_pair();
        expect(nothrow([&] { auto _ = connected_client{ std::move(socket) }; }));
    };

    wl_tag / "connected_client can reserve object id"_test = [] {
        auto client    = connected_client{};
        const auto id1 = client.reserve_object_id<protocols::wl_display>();
        const auto id2 = client.reserve_object_id<protocols::wl_touch>();
        const auto id3 = client.reserve_object_id();

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
        auto client = connected_client{};

        using namespace protocols;
        const auto registery = client.reserve_object_id<wl_registry>();
        const auto request   = wl_display::request::get_registry{ registery };

        expect(requires { client.register_request(global_display_object, request); });

        client.register_request(global_display_object, request);
    };

    wl_tag / "connected_client denies registeration for requests of different interface"_test = [] {
        auto client = connected_client{};

        using namespace protocols;
        const auto registery = client.reserve_object_id<wl_registry>();
        const auto request   = wl_display::request::get_registry{ registery };

        const auto wl_shm_object = client.reserve_object_id<wl_shm>();

        // Ugly hack to make program not ill-formed, as:
        //
        //   > If a substitution failure would occur in a requires-expression
        //   > for every possible template argument, the program is ill-formed,
        //   > no diagnostic required:
        //
        // so if expect(...) from the lambda would be outside the lambda,
        // it would be subtitution failure everytime -> ill-formed program and tests what is is supposed to.
        //
        // But now that it is generic lambda it is not neccessearly every
        // time, so this compiles.
        auto hack = [&]<typename T>(T obj) {
            expect(not requires { client.register_request(obj, request); });
        };
        hack(wl_shm_object);
    };

    wl_tag / "connected_client can flush of registered requests"_test = [] {
        auto client = connected_client{};
        expect(not client.has_registered_requests());

        using namespace protocols;
        const auto registery = client.reserve_object_id<wl_registry>();
        const auto request   = wl_display::request::get_registry{ registery };

        client.register_request(global_display_object, request);

        expect(client.has_registered_requests());

        client.flush_registered_requests();

        expect(not client.has_registered_requests());
    };
}
