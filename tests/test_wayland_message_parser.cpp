#include <boost/ut.hpp> // import boost.ut;

#include <cstddef>
#include <span>
#include <stdexcept>
#include <vector>

#include "bit_fiddling.hpp"
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

    wl_tag / "message_parser can handle empty input"_test = [] {
        const auto empty_vec = std::vector<std::byte>{};

        auto parser = wl::message_parser{ empty_vec };
        auto gen    = parser.message_generator();
        expect(gen.begin() == gen.end());
    };

    wl_tag / "message_parser can handle too short (invalid-)message at the beginning"_test = [] {
        const auto empty_vec = std::vector<std::byte>(5, std::byte{ 42 });

        auto parser = wl::message_parser{ empty_vec };
        auto gen    = parser.message_generator();
        expect(throws<std::logic_error>([&] { gen.begin(); }));
    };

    wl_tag / "message_parser can handle too short (invalid-)message at the end"_test = [] {
        using wl_display           = wl::protocols::wl_display;
        using get_registery        = wl_display::request::get_registry;
        constexpr auto header      = wl::message_header<wl_display>(wl::global_display_object,
                                                               get_registery::opcode,
                                                               { 12u });
        constexpr auto msg         = get_registery{ 2u };
        constexpr auto garbage_msg = 42u;
        auto get_registery_request = bit::byte_buff<16>{ header, msg, garbage_msg };

        auto parser       = wl::message_parser{ get_registery_request.bytes() };
        auto gen          = parser.message_generator();
        auto msg_iterator = gen.begin();

        expect(msg_iterator != gen.end());
        expect(throws<std::logic_error>([&] { ++msg_iterator; }));
    };

    wl_tag / "message_parser detects incorrect message sizes"_test = [] {
        using wl_display           = wl::protocols::wl_display;
        using get_registery        = wl_display::request::get_registry;
        constexpr auto header      = wl::message_header<wl_display>(wl::global_display_object,
                                                               get_registery::opcode,
                                                               { 5u });
        constexpr auto msg         = get_registery{ 2u };
        auto get_registery_request = bit::byte_buff<12>{ header, msg };

        auto parser       = wl::message_parser{ get_registery_request.bytes() };
        auto gen          = parser.message_generator();
        auto msg_iterator = gen.begin();

        expect(msg_iterator != gen.end());

        expect(throws<std::logic_error>([&] { ++msg_iterator; }));
    };

    wl_tag / "message_parser can parse object_id and opcode of message"_test = [] {
        using wl_display           = wl::protocols::wl_display;
        using get_registery        = wl_display::request::get_registry;
        constexpr auto header      = wl::message_header<wl_display>(wl::global_display_object,
                                                               get_registery::opcode,
                                                               { 12u });
        constexpr auto msg         = get_registery{ 2u };
        auto get_registery_request = bit::byte_buff<12>{ header, msg };

        auto parser       = wl::message_parser{ get_registery_request.bytes() };
        auto gen          = parser.message_generator();
        auto msg_iterator = gen.begin();

        expect(msg_iterator != gen.end());

        const auto parsed_msg = *msg_iterator;
        expect(parsed_msg.object_id == header.object_id);
        expect(parsed_msg.opcode == header.opcode);

        expect(++msg_iterator == gen.end());
    };

    wl_tag / "message_parser can parse object_id and opcode of two messages"_test = [] {
        using namespace wl::protocols;

        using get_registery     = wl_display::request::get_registry;
        constexpr auto header_A = wl::message_header<wl_display>(wl::global_display_object,
                                                                 get_registery::opcode,
                                                                 { 12u });
        constexpr auto msg_A    = get_registery{ 42u };

        using registery_bind = wl_registry::request::bind;
        constexpr auto header_B =
            wl::message_header<wl_registry>({ 2u }, registery_bind::opcode, { 16u });
        constexpr auto msg_B = registery_bind{ 43u, 44u };

        auto messages_AB = bit::byte_buff<28>{ header_A, msg_A, header_B, msg_B };

        auto parser       = wl::message_parser{ messages_AB.bytes() };
        auto gen          = parser.message_generator();
        auto msg_iterator = gen.begin();

        expect(msg_iterator != gen.end());

        const auto parsed_msg_A = *msg_iterator;
        expect(parsed_msg_A.object_id == header_A.object_id);
        expect(parsed_msg_A.opcode == header_A.opcode);

        expect(++msg_iterator != gen.end());

        const auto parsed_msg_B = *msg_iterator;
        expect(parsed_msg_B.object_id == header_B.object_id);
        expect(parsed_msg_B.opcode == header_B.opcode);

        expect(++msg_iterator == gen.end());
    };
}
