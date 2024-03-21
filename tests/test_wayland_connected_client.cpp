#include <boost/ut.hpp> // import boost.ut;

#include <type_traits>

#include "wayland/protocol.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger::wl;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "connected_client object can be constructor"_test = [] {
        expect(nothrow([] { auto _ = connected_client(); }));
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

}
