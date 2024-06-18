#include <boost/ut.hpp> // import boost.ut;

#include <algorithm>
#include <array>
#include <cstddef>
#include <span>
#include <stdexcept>
#include <vector>

#include "bit_fiddling.hpp"
#include "wayland/message_parser.hpp"
#include "wayland/protocol.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

/// Tests if `{ span } == { { subspan }, ... }`
constexpr auto is_beginning_subspan(const std::span<const std::byte> span,
                                    const std::span<const std::byte> subspan) {
    if (span.size() < subspan.size()) {
        // Subspan can not be longer than span.
        return false;
    }
    return std::ranges::equal(span.first(subspan.size()), subspan);
}

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

        auto parser = wl::message_parser{ get_registery_request.bytes() };
        auto gen    = parser.message_generator();

        expect(throws<std::logic_error>([&] { gen.begin(); }));
    };

    wl_tag / "message_parser can parse message"_test = [] {
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

        wl_tag / "and its payload/argument"_test = [&] {
            expect(is_beginning_subspan(
                parsed_msg.arguments,
                get_registery_request.bytes().subspan(sizeof(header), sizeof(msg))));
        };

        expect(++msg_iterator == gen.end());
    };

    wl_tag / "message_parser can parse object_id and opcode of two messages"_test = [&] {
        using namespace wl::protocols;

        // Scenario setup:

        using get_registery     = wl_display::request::get_registry;
        constexpr auto header_A = wl::message_header<wl_display>(wl::global_display_object,
                                                                 get_registery::opcode,
                                                                 { 12u });
        constexpr auto msg_A    = get_registery{ 42u };

        using registery_bind = wl_registry::request::bind;
        constexpr auto msg_B = registery_bind{ .name                     = { 43u },
                                               .new_id_interface         = {},
                                               .new_id_interface_version = { 999u },
                                               .id                       = { 10 } };

        constexpr auto header_B = wl::message_header<wl_registry>(
            { 2u },
            registery_bind::opcode,
            { 8 + sizeof(msg_B.name) + sizeof(wl::Wstring::size_type) + 4
              + sizeof(msg_B.new_id_interface_version) + sizeof(msg_B.id) }); // + 4 for the string

        constexpr auto B_str = std::array<std::byte, 4>{ std::byte{ 'g' },
                                                         std::byte{ 'e' },
                                                         std::byte{ 'r' },
                                                         std::byte{ '\0' } };

        // clang-format off
        auto messages_AB = bit::byte_buff<header_A.size.value + header_B.size.value>{
            header_A,
            msg_A,
            header_B,
            msg_B.name,
            wl::Wstring::size_type{ B_str.size() },
            B_str,
            msg_B.new_id_interface_version,
            msg_B.id
        };
        // clang-format on

        // Actual tests:

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

        wl_tag / "and their payloads/arguments"_test = [&] {
            expect(
                is_beginning_subspan(parsed_msg_A.arguments,
                                     messages_AB.bytes().subspan(sizeof(header_A), sizeof(msg_A))));
            constexpr auto B_arg_offset = sizeof(header_A) + sizeof(msg_A) + sizeof(header_B);
            expect(is_beginning_subspan(
                parsed_msg_B.arguments,
                messages_AB.bytes().subspan(B_arg_offset, header_B.size.value - 8)));
        };
    };
}
