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

#include <config.h>

#include <boost/ut.hpp> // import boost.ut;

#include <array>
#include <concepts>
#include <cstddef>
#include <memory>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

#include "gnu_utils/memory_block.hpp"
#include "sstd.hpp"
#include "wayland/message_buffer.hpp"
#include "wayland/message_parser.hpp"
#include "wayland/connected_client.hpp"
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
        constexpr auto mock_new_id = wl::Wnew_id<wl::protocols::wl_registry>{ 42u };
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

    wl_tag / "message with Wstring in message_buffer has correct header and size"_test = [&] {
        // Scenario setup:

        using display                = wl::protocols::wl_display;
        const auto mock_msg_with_str = display::event::error{ .object_id = { 4u },
                                                              .code      = { 42u },
                                                              .message   = { u8"display error" } };

        using type = std::remove_cvref_t<decltype(mock_msg_with_str.message)>;
        expect(std::is_same_v<type, wl::Wstring>);
        expect(sizeof(mock_msg_with_str.object_id) == 4uz);
        expect(sizeof(mock_msg_with_str.code) == 4uz);
        constexpr auto header_size = 8uz;
        const auto str_msg_argument_size_without_pad =
            sizeof(wl::Wstring::size_type) + mock_msg_with_str.message.size() + 1uz;
        const auto str_msg_argument_size =
            str_msg_argument_size_without_pad
            + sstd::round_upto_multiple_of<4>(str_msg_argument_size_without_pad);
        const auto expected_size = header_size + sizeof(mock_msg_with_str.object_id)
                                   + sizeof(mock_msg_with_str.code) + str_msg_argument_size;

        // Actual test:

        auto buff = wl::message_buffer{};
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

        wl_tag / "string argument is correct"_test = [&] {
            const auto index_of_msg_size_byte =
                header_size + sizeof(mock_msg_with_str.object_id) + sizeof(mock_msg_with_str.code);
            wl::Wstring::size_type size;
            std::memcpy(&size, std::addressof(released_data[index_of_msg_size_byte]), sizeof(size));

            expect(size == mock_msg_with_str.message.size() + 1); // + 1 for null delimiter.
            expect(size != str_msg_argument_size);
            expect(size != str_msg_argument_size_without_pad);

            const auto str_in_buff = std::span<const std::byte>(
                std::addressof(
                    released_data[index_of_msg_size_byte + sizeof(wl::Wstring::size_type)]),
                mock_msg_with_str.message.size());
            const auto expected_str = std::span<const std::byte>(
                reinterpret_cast<std::byte const*>(mock_msg_with_str.message.data()),
                mock_msg_with_str.message.size());
            expect(std::ranges::equal(str_in_buff, expected_str));

            const auto expected_null_from_buff =
                released_data[index_of_msg_size_byte + sizeof(size) + size - 1];
            expect(expected_null_from_buff == std::byte{ 0 });
        };
    };

    wl_tag / "message with Warray in message_buffer has correct header and size"_test = [] {
        // Scenario setup:

        const auto mock_data    = std::array{ std::byte{ 1 }, std::byte{ 2 }, std::byte{ 3 } };
        using keyboard          = wl::protocols::wl_keyboard;
        const auto keyboard_obj = wl::Wobject<keyboard>{ 2u };
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

        // Actual tests:

        auto buff = wl::message_buffer{};
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

        wl_tag / "array argument is correct"_test = [&] {
            const auto index_of_array_size_byte = header_size + sizeof(mock_msg_with_array.serial)
                                                  + sizeof(mock_msg_with_array.surface);
            wl::Warray::size_type size;
            std::memcpy(&size,
                        std::addressof(released_data[index_of_array_size_byte]),
                        sizeof(size));

            expect(size == mock_msg_with_array.keys.size());
            expect(size != array_size);
            expect(size != padded_array_size);

            const auto array_in_buff = std::span<const std::byte>(
                std::addressof(
                    released_data[index_of_array_size_byte + sizeof(wl::Warray::size_type)]),
                mock_msg_with_array.keys.size());
            expect(std::ranges::equal(array_in_buff, mock_msg_with_array.keys));
        };
    };

    wl_tag / "message with short Wstring in message_buffer is padded correctly "_test = [] {
        // Scenario setup:

        using display = wl::protocols::wl_display;
        const auto mock_msg_with_str =
            display::event::error{ .object_id = { 4u }, .code = { 42u }, .message = { u8"M" } };

        using type = std::remove_cvref_t<decltype(mock_msg_with_str.message)>;
        expect(std::is_same_v<type, wl::Wstring>);
        expect(sizeof(mock_msg_with_str.object_id) == 4uz);
        expect(sizeof(mock_msg_with_str.code) == 4uz);
        constexpr auto header_size = 8uz;
        const auto str_msg_argument_size_without_pad =
            sizeof(wl::Wstring::size_type) + mock_msg_with_str.message.size() + 1uz;
        const auto str_msg_argument_size =
            str_msg_argument_size_without_pad
            + sstd::round_upto_multiple_of<4>(str_msg_argument_size_without_pad);

        // Actual tests:

        auto buff = wl::message_buffer{};
        buff.append(wl::global_display_object, mock_msg_with_str);
        const auto released_data = buff.release_data();

        expect(released_data.size() % 4uz == 0);

        wl_tag / "string argument is correct"_test = [&] {
            const auto index_of_msg_size_byte =
                header_size + sizeof(mock_msg_with_str.object_id) + sizeof(mock_msg_with_str.code);
            wl::Wstring::size_type size;
            std::memcpy(&size, std::addressof(released_data[index_of_msg_size_byte]), sizeof(size));

            expect(size == mock_msg_with_str.message.size() + 1); // + 1 for null delimiter.
            expect(size != str_msg_argument_size);
            expect(size != str_msg_argument_size_without_pad);

            const auto str_in_buff = std::span<const std::byte>(
                std::addressof(
                    released_data[index_of_msg_size_byte + sizeof(wl::Wstring::size_type)]),
                mock_msg_with_str.message.size());
            const auto expected_str = std::span<const std::byte>(
                reinterpret_cast<std::byte const*>(mock_msg_with_str.message.data()),
                mock_msg_with_str.message.size());
            expect(std::ranges::equal(str_in_buff, expected_str));
        };
    };

    wl_tag / "message with short Warray in message_buffer is padded correctly"_test = [] {
        // Scenario setup:

        const auto mock_data    = std::array{ std::byte{ 1 } };
        using keyboard          = wl::protocols::wl_keyboard;
        const auto keyboard_obj = wl::Wobject<keyboard>{ 2u };
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

        // Actual tests:

        auto buff = wl::message_buffer{};
        buff.append(keyboard_obj, mock_msg_with_array);

        expect(not buff.empty());
        const auto released_data = buff.release_data();
        expect(released_data.size() % 4uz == 0uz);

        wl_tag / "array argument is correct"_test = [&] {
            const auto index_of_array_size_byte = header_size + sizeof(mock_msg_with_array.serial)
                                                  + sizeof(mock_msg_with_array.surface);
            wl::Warray::size_type size;
            std::memcpy(&size,
                        std::addressof(released_data[index_of_array_size_byte]),
                        sizeof(size));

            expect(size == mock_msg_with_array.keys.size());
            expect(size != array_size);
            expect(size != padded_array_size);

            const auto array_in_buff = std::span<const std::byte>(
                std::addressof(
                    released_data[index_of_array_size_byte + sizeof(wl::Warray::size_type)]),
                mock_msg_with_array.keys.size());
            expect(std::ranges::equal(array_in_buff, mock_msg_with_array.keys));
        };
    };

    wl_tag / "message_buffer queues given Wfd"_test = [] {
        // Scenario setup:

        using wl_shm             = wl::protocols::wl_shm;
        const auto shm_object_id = wl::Wobject<wl_shm>{ 2u };
        auto mem                 = gnu::memory_block{};
        const auto mem_ref       = gnu::fd_ref{ mem };
        const auto mock_msg_with_fd =
            wl_shm::request::create_pool{ .id{ 42u }, .fd{ mem_ref }, .size{ 10 } };

        // Actual tests:

        auto buff = wl::message_buffer{};
        buff.append(shm_object_id, mock_msg_with_fd);

        expect(not buff.empty());
        std::ignore = buff.release_data();
        expect(not buff.empty()) << "Should contain queued file descriptor.";
        auto fd_vec = buff.release_fds();
        expect(buff.empty());

        expect(fatal(fd_vec.size() == 1));
        // This test is little lacking as it does not test if the queued fd is actually
        // the one that was given to the test. This because Guilander does not support
        // receiving Wfd primitives at the moment, so there is no utility to do anything
        // with a Wfd object.
    };
}
