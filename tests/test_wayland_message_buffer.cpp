#include <boost/ut.hpp> // import boost.ut;

#include <array>
#include <concepts>
#include <cstddef>
#include <memory>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

#include "sstd.hpp"
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

    wl_tag / "messages of constant size in message_buffer have correct header and size"_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());

        const auto mock_request = wl::protocols::wl_display::request::get_registry{ 2u };
        const auto mock_event   = wl::protocols::wl_display::event::delete_id{ 4u };
        expect(sizeof(mock_request) == std::integral_constant<std::size_t, 4>::value);
        expect(sizeof(mock_event) == std::integral_constant<std::size_t, 4>::value);
        buff.append(wl::global_display_object, mock_request);
        buff.append(wl::global_display_object, mock_event);

        constexpr auto header_size   = 8uz;
        constexpr auto expected_size = 2 * header_size + sizeof(mock_request) + sizeof(mock_event);

        expect(not buff.empty());
        const auto released_data = buff.release_data();
        expect(buff.empty());
        expect(released_data.size() == std::integral_constant<std::size_t, expected_size>::value);

        auto msg_parser = wl::message_parser{ released_data };
        auto msg_gen    = msg_parser.message_generator();
        auto msg_iter   = msg_gen.begin();

        expect(msg_iter != msg_gen.end());
        expect((*msg_iter).object_id == wl::global_display_object);
        expect((*msg_iter).opcode == wl::protocols::wl_display::request::get_registry::opcode);

        ++msg_iter;
        expect((*msg_iter).object_id == wl::global_display_object);
        expect((*msg_iter).opcode == wl::protocols::wl_display::event::delete_id::opcode);
        expect(++msg_iter == msg_gen.end());
    };

    wl_tag / "message with Wstring in message_buffer has correct header and size"_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());

        // Message with Wstring:
        using display                = wl::protocols::wl_display;
        const auto mock_msg_with_str = display::event::error{ .object_id = { 4u },
                                                              .code      = { 42u },
                                                              .message   = { u8"display error" } };

        using type = std::remove_cvref_t<decltype(mock_msg_with_str.message)>;
        expect(std::is_same_v<type, wl::Wstring>);
        expect(sizeof(mock_msg_with_str.object_id) == 4uz);
        expect(sizeof(mock_msg_with_str.code) == 4uz);
        constexpr auto header_size = 8uz;
        const auto str_size_without_pad =
            sizeof(wl::Wstring::size_type) + mock_msg_with_str.message.size() + 1uz;
        const auto str_size =
            str_size_without_pad + sstd::round_upto_multiple_of<4>(str_size_without_pad);
        const auto expected_size = header_size + sizeof(mock_msg_with_str.object_id)
                                   + sizeof(mock_msg_with_str.code) + str_size;

        buff.append(wl::global_display_object, mock_msg_with_str);

        expect(not buff.empty());
        const auto released_data = buff.release_data();
        expect(buff.empty());
        expect(released_data.size() == expected_size);

        auto msg_parser = wl::message_parser{ released_data };
        auto msg_gen    = msg_parser.message_generator();
        auto msg_iter   = msg_gen.begin();

        expect(msg_iter != msg_gen.end());
        expect((*msg_iter).object_id == wl::global_display_object);
        expect((*msg_iter).opcode == display::event::error::opcode);
        expect(++msg_iter == msg_gen.end());
    };

    wl_tag / "message with Warray in message_buffer has correct header and size"_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());

        // Message with Warray:
        const auto mock_data = std::array{ std::byte{ 1 }, std::byte{ 2 }, std::byte{ 3 } };
        using keyboard       = wl::protocols::wl_keyboard;
        const auto mock_msg_with_array =
            keyboard::event::enter{ .serial  = { 5u },
                                    .surface = { 43u },
                                    .keys    = { std::span{ mock_data } } };

        using type = std::remove_cvref_t<decltype(mock_msg_with_array.keys)>;
        expect(std::is_same_v<type, wl::Warray>);
        expect(sizeof(mock_msg_with_array.serial) == 4uz);
        expect(sizeof(mock_msg_with_array.surface) == 4uz);
        constexpr auto header_size = 8uz;
        const auto array_size = sizeof(wl::Warray::size_type) + mock_msg_with_array.keys.size();
        const auto padded_array_size = array_size + sstd::round_upto_multiple_of<4>(array_size);
        const auto expected_size     = header_size + sizeof(mock_msg_with_array.serial)
                                   + sizeof(mock_msg_with_array.surface) + padded_array_size;

        expect(std::is_same_v<wl::Warray, std::remove_cvref_t<decltype(mock_msg_with_array.keys)>>);
        const auto keyboard_obj = wl::Wobject<keyboard>{ 2u };
        buff.append(keyboard_obj, mock_msg_with_array);

        expect(not buff.empty());
        const auto released_data = buff.release_data();
        expect(buff.empty());
        expect(released_data.size() == expected_size);

        auto msg_parser = wl::message_parser{ released_data };
        auto msg_gen    = msg_parser.message_generator();
        auto msg_iter   = msg_gen.begin();

        expect(msg_iter != msg_gen.end());
        expect((*msg_iter).object_id == keyboard_obj);
        expect((*msg_iter).opcode == keyboard::event::enter::opcode);
        expect(++msg_iter == msg_gen.end());
    };

    wl_tag / "message with short Wstring in padded correctly "_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());

        // Message with Wstring:
        using display = wl::protocols::wl_display;
        const auto mock_msg_with_str =
            display::event::error{ .object_id = { 4u }, .code = { 42u }, .message = { u8"M" } };

        using type = std::remove_cvref_t<decltype(mock_msg_with_str.message)>;
        expect(std::is_same_v<type, wl::Wstring>);
        expect(sizeof(mock_msg_with_str.object_id) == 4uz);
        expect(sizeof(mock_msg_with_str.code) == 4uz);
        constexpr auto header_size = 8uz;
        const auto str_size_without_pad =
            sizeof(wl::Wstring::size_type) + mock_msg_with_str.message.size() + 1uz;
        const auto str_size =
            str_size_without_pad + sstd::round_upto_multiple_of<4>(str_size_without_pad);

        buff.append(wl::global_display_object, mock_msg_with_str);

        expect(not buff.empty());
        const auto released_data = buff.release_data();
        expect(released_data.size() % 4uz == 0);

        const auto index_of_msg_size_byte =
            header_size + sizeof(mock_msg_with_str.object_id) + sizeof(mock_msg_with_str.code);
        wl::Wstring::size_type size;
        std::memcpy(&size, std::addressof(released_data[index_of_msg_size_byte]), sizeof(size));
        expect(size == str_size_without_pad);
        expect(size != str_size);
    };

    wl_tag / "message with Warray in message_buffer has correct header and size"_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());

        // Message with Warray:
        const auto mock_data = std::array{ std::byte{ 1 }, std::byte{ 2 }, std::byte{ 3 } };
        using keyboard       = wl::protocols::wl_keyboard;
        const auto mock_msg_with_array =
            keyboard::event::enter{ .serial  = { 5u },
                                    .surface = { 43u },
                                    .keys    = { std::span{ mock_data } } };

        using type = std::remove_cvref_t<decltype(mock_msg_with_array.keys)>;
        expect(std::is_same_v<type, wl::Warray>);
        expect(sizeof(mock_msg_with_array.serial) == 4uz);
        expect(sizeof(mock_msg_with_array.surface) == 4uz);
        constexpr auto header_size = 8uz;
        const auto array_size = sizeof(wl::Warray::size_type) + mock_msg_with_array.keys.size();
        const auto padded_array_size = array_size + sstd::round_upto_multiple_of<4>(array_size);
        const auto expected_size     = header_size + sizeof(mock_msg_with_array.serial)
                                   + sizeof(mock_msg_with_array.surface) + padded_array_size;

        expect(std::is_same_v<wl::Warray, std::remove_cvref_t<decltype(mock_msg_with_array.keys)>>);
        const auto keyboard_obj = wl::Wobject<keyboard>{ 2u };
        buff.append(keyboard_obj, mock_msg_with_array);

        expect(not buff.empty());
        const auto released_data = buff.release_data();
        expect(buff.empty());
        expect(released_data.size() == expected_size);

        auto msg_parser = wl::message_parser{ released_data };
        auto msg_gen    = msg_parser.message_generator();
        auto msg_iter   = msg_gen.begin();

        expect(msg_iter != msg_gen.end());
        expect((*msg_iter).object_id == keyboard_obj);
        expect((*msg_iter).opcode == keyboard::event::enter::opcode);
        expect(++msg_iter == msg_gen.end());
    };

    wl_tag / "message with short Warray in message_buffer is padded correctly"_test = [] {
        auto buff = wl::message_buffer{};
        expect(buff.empty());

        // Message with Warray:
        const auto mock_data = std::array{ std::byte{ 1 } };
        using keyboard       = wl::protocols::wl_keyboard;
        const auto mock_msg_with_array =
            keyboard::event::enter{ .serial  = { 5u },
                                    .surface = { 43u },
                                    .keys    = { std::span{ mock_data } } };

        using type = std::remove_cvref_t<decltype(mock_msg_with_array.keys)>;
        expect(std::is_same_v<type, wl::Warray>);
        expect(sizeof(mock_msg_with_array.serial) == 4uz);
        expect(sizeof(mock_msg_with_array.surface) == 4uz);
        constexpr auto header_size = 8uz;
        const auto array_size = sizeof(wl::Warray::size_type) + mock_msg_with_array.keys.size();
        const auto padded_array_size = array_size + sstd::round_upto_multiple_of<4>(array_size);

        expect(std::is_same_v<wl::Warray, std::remove_cvref_t<decltype(mock_msg_with_array.keys)>>);
        const auto keyboard_obj = wl::Wobject<keyboard>{ 2u };
        buff.append(keyboard_obj, mock_msg_with_array);

        expect(not buff.empty());
        const auto released_data = buff.release_data();
        expect(released_data.size() % 4uz == 0uz);

        const auto index_of_array_size_byte =
            header_size + sizeof(mock_msg_with_array.serial) + sizeof(mock_msg_with_array.surface);
        wl::Warray::size_type size;
        std::memcpy(&size, std::addressof(released_data[index_of_array_size_byte]), sizeof(size));
        expect(size == array_size);
        expect(size != padded_array_size);
    };
}
