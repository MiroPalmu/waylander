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

#include <algorithm>
#include <optional>
#include <ranges>
#include <span>
#include <vector>

#include "waylander/sstd.hpp"
#include "waylander/wayland/connected_client.hpp"
#include "waylander/wayland/message_parser.hpp"
#include "waylander/wayland/message_visitor.hpp"
#include "waylander/wayland/protocol_primitives.hpp"
#include "waylander/wayland/protocols/wayland_protocol.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "message_visit uses the default overload without arguments"_test = [] {
        // Scenario setup:

        using touch = wl::protocols::wl_touch;
        using up    = touch::event::up;

        constexpr auto touch_obj = wl::Wobject<touch>{ 42 };

        // UM := Unkown Message
        constexpr auto UM_obj_id = wl::Wobject<wl::generic_object>{ touch_obj.value };
        constexpr auto UM_opcode = wl::Wopcode<wl::generic_object>{ up::opcode.value };
        constexpr up UM_arguments_arr[]{ { .serial{ 0 }, .time{ 0 }, .id{ 0 } } };
        const auto UM_arguments = std::as_bytes(std::span(UM_arguments_arr));
        static_assert(sizeof(up) % 4 == 0, "Needs padding to be correct message.");

        const auto UM = wl::parsed_message{ UM_obj_id, UM_opcode, UM_arguments };

        // Actual tests:

        auto default_counter             = 0uz;
        auto default_counter_incrementer = [&] { ++default_counter; };
        auto empty_overloads             = wl::message_overload_set{};

        wl::message_visit(default_counter_incrementer, empty_overloads, UM);
        expect(default_counter == 1);

        wl::message_visit(default_counter_incrementer, empty_overloads, UM);
        wl::message_visit(default_counter_incrementer, empty_overloads, UM);
        expect(default_counter == 3);
    };

    wl_tag / "message_visit uses the default overload with parsed_message argument"_test = [] {
        // Scenario setup:

        using touch = wl::protocols::wl_touch;
        using up    = touch::event::up;

        constexpr auto touch_obj = wl::Wobject<touch>{ 42 };

        // UM := Unkown Message
        constexpr auto UM_obj_id = wl::Wobject<wl::generic_object>{ touch_obj.value };
        constexpr auto UM_opcode = wl::Wopcode<wl::generic_object>{ up::opcode.value };
        constexpr up UM_arguments_arr[]{ { .serial{ 0 }, .time{ 0 }, .id{ 0 } } };
        const auto UM_arguments = std::as_bytes(std::span(UM_arguments_arr));
        static_assert(sizeof(up) % 4 == 0, "Needs padding to be correct message.");

        const auto UM = wl::parsed_message{ UM_obj_id, UM_opcode, UM_arguments };

        // Actual tests:

        auto default_counter             = 0uz;
        auto default_counter_incrementer = [&](const wl::parsed_message& msg) {
            expect(msg.object_id == touch_obj);
            expect(msg.opcode == up::opcode);
            expect(std::ranges::equal(msg.arguments, UM_arguments));
            ++default_counter;
        };
        auto empty_overloads = wl::message_overload_set{};

        wl::message_visit(default_counter_incrementer, empty_overloads, UM);
        expect(default_counter == 1);

        wl::message_visit(default_counter_incrementer, empty_overloads, UM);
        wl::message_visit(default_counter_incrementer, empty_overloads, UM);
        expect(default_counter == 3);
    };

    wl_tag / "message_visit uses overload for static message"_test = [] {
        // Scenario setup:

        using touch = wl::protocols::wl_touch;
        using up    = touch::event::up;

#ifndef __clang__
        // This will crash LLVM 17 and 18 (annoyingly will break clangd), see:
        // https://github.com/llvm/llvm-project/issues/93821#issue-2325716041
        static_assert(wl::static_message<up>, "Test assumes this to be true.");
#endif

        constexpr auto touch_obj = wl::Wobject<touch>{ 42 };

        // UM = Unkown Message
        constexpr auto UM_obj_id = wl::Wobject<wl::generic_object>{ touch_obj.value };
        constexpr auto UM_opcode = wl::Wopcode<wl::generic_object>{ up::opcode.value };
        constexpr up UM_arguments_arr[]{ { .serial{ 0 }, .time{ 0 }, .id{ 0 } } };
        const auto UM_arguments = std::as_bytes(std::span(UM_arguments_arr));

        static_assert(sizeof(up) % 4 == 0, "Needs padding to be correct message.");

        const auto UM = wl::parsed_message{ UM_obj_id, UM_opcode, UM_arguments };

        // Actual tests:

        auto default_counter = 0uz;
        auto up_counter      = 0uz;

        auto overload_set                = wl::message_overload_set{};
        auto default_counter_incrementer = [&] { ++default_counter; };

        wl::message_visit(default_counter_incrementer, overload_set, UM);
        expect(default_counter == 1);
        expect(up_counter == 0);

        overload_set.add_overload<up>(touch_obj, [&](const up& from_visit) {
            ++up_counter;
            expect(from_visit.serial == UM_arguments_arr[0].serial);
            expect(from_visit.time == UM_arguments_arr[0].time);
            expect(from_visit.id == UM_arguments_arr[0].id);
        });

        wl::message_visit(default_counter_incrementer, overload_set, UM);
        expect(default_counter == 1);
        expect(up_counter == 1);

        const auto other_UM_obj_id = wl::Wobject<wl::generic_object>{ UM_obj_id.value + 1 };
        const auto other_UM        = wl::parsed_message{ other_UM_obj_id, UM_opcode, UM_arguments };

        wl::message_visit(default_counter_incrementer, overload_set, other_UM);
        expect(default_counter == 2);
        expect(up_counter == 1);
    };

    wl_tag / "message_visit uses added overloads for message with W[array|string]"_test = [] {
        // Used interfaces.
        using wl_display  = wl::protocols::wl_display;
        using wl_keyboard = wl::protocols::wl_keyboard;

        // Used messages.
        using msg_str_t = wl_display::event::error;
        using msg_arr_t = wl_keyboard::event::enter;

        // Data for the messages.
        constexpr std::byte some_data[]{ std::byte{ 'p' }, std::byte{ 'i' }, std::byte{ 'r' },
                                         std::byte{ 'a' }, std::byte{ 't' }, std::byte{ 'e' } };

        // "Name" the different object ids used.
        constexpr auto display_obj    = wl::global_display_object;
        constexpr auto keyboard_obj_A = wl::Wobject<wl_keyboard>{ 123 };
        constexpr auto keyboard_obj_B = wl::Wobject<wl_keyboard>{ 321 };
        constexpr auto keyboard_obj_C = wl::Wobject<wl_keyboard>{ 333 };

        // Assume that message_[parser|buffer] works and construct messages:
        //
        // 1. display_obj:    msg_str_t{ 42, 43, "FooBar" }
        // 2. keyboard_obj_A: msg_arr_t{ 12, 13, { pirate } }
        // 3. display_obj:    msg_str_t{ 43, 44, "Bar" }
        // 4. keyboard_obj_B: msg_arr_t{ 21, 31, { pir } }
        // 5. keyboard_obj_C: msg_arr_t{ 21, 31, { pir } }
        auto msg_parser = [&]() -> wl::message_parser {
            auto buff = wl::message_buffer{};
            buff.append(display_obj,
                        msg_str_t{ .object_id{ 42 }, .code{ 43 }, .message{ u8"FooBar" } });

            buff.append(keyboard_obj_A,
                        msg_arr_t{ .serial{ 12 }, .surface{ 13 }, .keys{ some_data } });

            buff.append(display_obj,
                        msg_str_t{ .object_id{ 43 }, .code{ 44 }, .message{ u8"Bar" } });

            buff.append(
                keyboard_obj_B,
                msg_arr_t{ .serial{ 21 }, .surface{ 31 }, .keys{ std::span(some_data).first(3) } });

            buff.append(
                keyboard_obj_C,
                msg_arr_t{ .serial{ 21 }, .surface{ 31 }, .keys{ std::span(some_data).first(3) } });

            return { buff.release_data() };
        }();

        auto default_counter        = 0uz;
        auto str_counter            = 0uz;
        auto arr_counter            = 0uz;
        const auto default_overload = [&](const wl::parsed_message& msg) {
            ++default_counter;
            expect(msg.object_id == keyboard_obj_C);
            expect(msg.opcode == msg_arr_t::opcode);
        };

        auto overload_set = wl::message_overload_set{};
        overload_set.add_overload<msg_str_t>(display_obj, [&](const msg_str_t& msg) {
            if (str_counter == 0) {
                expect(msg.object_id == 42);
                expect(msg.code == 43);
                expect(msg.message == u8"FooBar");
            } else if (str_counter == 1) {
                expect(msg.object_id == 43);
                expect(msg.code == 44);
                expect(msg.message == u8"Bar");
            }
            ++str_counter;
        });

        overload_set.add_overload<msg_arr_t>(keyboard_obj_A, [&](const msg_arr_t& msg) {
            ++arr_counter;
            expect(msg.serial == 12);
            expect(msg.surface == 13);
            expect(std::ranges::equal(msg.keys, some_data));
        });

        overload_set.add_overload<msg_arr_t>(keyboard_obj_B, [&](const msg_arr_t& msg) {
            arr_counter += 10;
            expect(msg.serial == 21);
            expect(msg.surface == 31);
            expect(std::ranges::equal(msg.keys, std::span(some_data).first(3)));
        });

        auto msg_gen = msg_parser.message_generator();
        auto msg_ptr = msg_gen.begin();

        expect(default_counter == 0);
        expect(str_counter == 0);
        expect(arr_counter == 0);

        wl::message_visit(default_overload, overload_set, *msg_ptr);
        ++msg_ptr;

        expect(default_counter == 0);
        expect(str_counter == 1);
        expect(arr_counter == 0);

        wl::message_visit(default_overload, overload_set, *msg_ptr);
        ++msg_ptr;

        expect(default_counter == 0);
        expect(str_counter == 1);
        expect(arr_counter == 1);

        wl::message_visit(default_overload, overload_set, *msg_ptr);
        ++msg_ptr;

        expect(default_counter == 0);
        expect(str_counter == 2);
        expect(arr_counter == 1);

        wl::message_visit(default_overload, overload_set, *msg_ptr);
        ++msg_ptr;

        expect(default_counter == 0);
        expect(str_counter == 2);
        expect(arr_counter == 11);

        wl::message_visit(default_overload, overload_set, *msg_ptr);

        expect(default_counter == 1);
        expect(str_counter == 2);
        expect(arr_counter == 11);

        expect(++msg_ptr == msg_gen.end());
    };

    wl_tag / "message_visit overloads can hold a state"_test = [] {
        // Secenario setup:

        using wl_display   = wl::protocols::wl_display;
        using get_registry = wl_display::request::get_registry;
        using wl_registry  = wl::protocols::wl_registry;
        using bind         = wl_registry::request::bind;

        constexpr auto registry_obj = wl::Wobject<wl_registry>{ 42 };
        constexpr auto display_obj  = wl::Wobject<wl_display>{ 55 };

        // Assume that message_[parser|buffer] works and construct messages:
        //
        // 1. wl::global_display_object: get_registry{ 1 }
        // 2. registry_obj: bind{ 2, "abc", 42, 3 }
        // 3. wl::global_display_object: get_registry{ 4 }
        // 4. registry_obj: bind{ 5, "def", 43, 6 }
        // 5. display_obj: get_registry{ 7 }
        // 6. display_obj: get_registry{ 7 }
        auto msg_parser = wl::message_parser{ [&] {
            auto buff   = wl::message_buffer{};
            buff.append(wl::global_display_object, get_registry{ .registry{ 1 } });
            buff.append(registry_obj,
                        bind{ .name{ 2 },
                              .new_id_interface{ u8"abc" },
                              .new_id_interface_version{ 42 },
                              .id{ 3 } });
            buff.append(wl::global_display_object, get_registry{ .registry{ 4 } });
            buff.append(registry_obj,
                        bind{ .name{ 5 },
                              .new_id_interface{ u8"def" },
                              .new_id_interface_version{ 43 },
                              .id{ 6 } });
            buff.append(display_obj, get_registry{ .registry{ 7 } });
            buff.append(display_obj, get_registry{ .registry{ 7 } });
            return buff.release_data();
        }() };

        // Actual tests:

        auto default_count = 0uz;
        auto request_count = 0uz;
        auto event_count   = 0uz;

        struct {
            std::size_t& default_count_ref;
            using prev_t = std::optional<std::vector<std::byte>>;
            prev_t previous_arguments{};

            void operator()(const wl::parsed_message& msg) {
                ++default_count_ref;
                if (not previous_arguments.has_value()) {
                    previous_arguments = std::vector(msg.arguments.begin(), msg.arguments.end());
                } else {
                    default_count_ref += 10;
                    expect(std::ranges::equal(previous_arguments.value(), msg.arguments));
                }
            }
        } default_overload{ default_count };

        auto overload_set = wl::message_overload_set{};

        overload_set.add_overload<get_registry>(
            wl::global_display_object,
            [&, first_time = true](const get_registry msg) mutable {
                if (first_time) {
                    first_time = false;
                    expect(msg.registry == 1);
                } else {
                    expect(msg.registry == 4);
                }
                ++request_count;
            });

        overload_set.add_overload<bind>(registry_obj,
                                        [&, first_time = true](const bind msg) mutable {
                                            if (first_time) {
                                                first_time = false;
                                                expect(msg.name == 2);
                                                expect(msg.new_id_interface == u8"abc");
                                                expect(msg.new_id_interface_version == 42);
                                                expect(msg.id == 3);
                                            } else {
                                                expect(msg.name == 5);
                                                expect(msg.new_id_interface == u8"def");
                                                expect(msg.new_id_interface_version == 43);
                                                expect(msg.id == 6);
                                            }
                                            ++event_count;
                                        });

        wl::message_visit(default_overload, overload_set, msg_parser.message_generator());
        expect(default_count == 12);
    };
}
