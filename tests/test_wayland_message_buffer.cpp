#include <boost/ut.hpp> // import boost.ut;

#include <concepts>
#include <ranges>
#include <tuple>
#include <utility>

#include "wayland/message_buffer.hpp"
#include "wayland/message_parser.hpp"
#include "wayland/protocol.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "empty message_buffer can be constructer"_test = [] {
        auto buff = wl::message_buffer{};

        expect(buff.empty());
    };

    wl_tag / "request messages can be appended to message_buffer to make it non-empty"_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());
        constexpr auto mock_new_id = wl::Wnew_id{ 42u };
        const auto mock_request_for_display =
            wl::protocols::wl_display::request::get_registry{ mock_new_id };

        buff.append(wl::global_display_object, mock_request_for_display);
        expect(not buff.empty());
    };

    wl_tag / "event messages can be appended to message_buffer to make it non-empty"_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());
        const auto mock_event_from_display = wl::protocols::wl_display::event::delete_id{ 4u };
        buff.append(wl::global_display_object, mock_event_from_display);
        expect(not buff.empty());
    };

    wl_tag / "message_buffer can release data"_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());

        const auto mock_request = wl::protocols::wl_display::request::get_registry{ 2u };
        const auto mock_event   = wl::protocols::wl_display::event::delete_id{ 4u };
        buff.append(wl::global_display_object, mock_request);
        buff.append(wl::global_display_object, mock_event);

        expect(not buff.empty());
        std::ignore = buff.release_data();
        expect(buff.empty());
    };

    wl_tag / "messages of constant size in message_buffer have correct header"_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());

        const auto mock_request = wl::protocols::wl_display::request::get_registry{ 2u };
        const auto mock_event   = wl::protocols::wl_display::event::delete_id{ 4u };
        buff.append(wl::global_display_object, mock_request);
        buff.append(wl::global_display_object, mock_event);

        expect(not buff.empty());
        auto msg_parser = wl::message_parser{ buff.release_data() };
        expect(buff.empty());

        auto msg_gen  = msg_parser.message_generator();
        auto msg_iter = msg_gen.begin();
        expect(msg_iter != msg_gen.end());

        expect((*msg_iter).object_id == wl::global_display_object);
        expect((*msg_iter).opcode == wl::protocols::wl_display::request::get_registry::opcode);
        ++msg_iter;
        expect((*msg_iter).object_id == wl::global_display_object);
        expect((*msg_iter).opcode == wl::protocols::wl_display::event::delete_id::opcode);

        expect(++msg_iter == msg_gen.end());
    };
}
