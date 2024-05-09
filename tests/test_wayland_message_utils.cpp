#include <boost/ut.hpp> // import boost.ut;

#include "wayland/message_utils.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

using wl_display = ger::wl::protocols::wl_display;
// using get_registery = wl_display::request::get_registry;

int main() {
    using namespace boost::ut;
    using namespace ger;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "static_message_argument is satisfied only by static Wayland primitives"_test = [] {
        // Static Wayland primitives.
        expect(wl::static_message_argument<wl::Wint>);
        expect(wl::static_message_argument<wl::Wuint>);
        expect(wl::static_message_argument<wl::Wfixed>);
        expect(wl::static_message_argument<wl::Wobject<>>);
        expect(wl::static_message_argument<wl::Wobject<wl_display>>);
        expect(wl::static_message_argument<wl::Wnew_id<>>);
        expect(wl::static_message_argument<wl::Wnew_id<wl_display>>);

        // Dynamic Wayland primitives.
        expect(not wl::static_message_argument<wl::Wstring>);
        expect(not wl::static_message_argument<wl::Warray>);
        expect(not wl::static_message_argument<wl::Wfd>);

        // Random types.
        expect(not wl::static_message_argument<int>);
        expect(not wl::static_message_argument<double[10]>);
        expect(not wl::static_message_argument<void>);
        struct Foo {
            int a;
            float b;
            double c;
        };
        expect(not wl::static_message_argument<Foo>);
    };

    wl_tag / "dynamic_message_argument is satisfied only by dynamic Wayland primitives"_test = [] {
        // Static Wayland primitives.
        expect(not wl::dynamic_message_argument<wl::Wint>);
        expect(not wl::dynamic_message_argument<wl::Wuint>);
        expect(not wl::dynamic_message_argument<wl::Wfixed>);
        expect(not wl::dynamic_message_argument<wl::Wobject<>>);
        expect(not wl::dynamic_message_argument<wl::Wobject<wl_display>>);
        expect(not wl::dynamic_message_argument<wl::Wnew_id<>>);
        expect(not wl::dynamic_message_argument<wl::Wnew_id<wl_display>>);

        // Dynamic Wayland primitives.
        expect(wl::dynamic_message_argument<wl::Wstring>);
        expect(wl::dynamic_message_argument<wl::Warray>);
        expect(wl::dynamic_message_argument<wl::Wfd>);

        // Random types.
        expect(not wl::dynamic_message_argument<int>);
        expect(not wl::dynamic_message_argument<double[10]>);
        expect(not wl::dynamic_message_argument<void>);
        struct Foo {
            int a;
            float b;
            double c;
        };
        expect(not wl::dynamic_message_argument<Foo>);
    };
}
