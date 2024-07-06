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

#include "wayland/message_utils.hpp"
#include "wayland/protocol_primitives.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

using wl_display    = ger::wl::protocols::wl_display;
using get_registery = wl_display::request::get_registry;

using data_src      = ger::wl::protocols::wl_data_source;
using data_src_dtro = ger::wl::protocols::wl_data_source;

using wl_shm        = ger::wl::protocols::wl_shm;
using wl_shm_pool   = ger::wl::protocols::wl_shm_pool;
using create_buffer = wl_shm_pool::request::create_buffer;

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

    wl_tag / "Wayland enums are static message arguments"_test = [] {
        using A = wl::protocols::wl_display::error;
        using B = wl::protocols::wl_shm::format;
        using C = wl::protocols::wl_keyboard::key_state;

        enum class D : std::uint16_t { e1, e2, e3 };

        expect(constant<wl::static_message_argument<A>>);
        expect(constant<wl::static_message_argument<B>>);
        expect(constant<wl::static_message_argument<C>>);
        expect(constant<not wl::static_message_argument<D>>);

        expect(constant<not wl::dynamic_message_argument<A>>);
        expect(constant<not wl::dynamic_message_argument<B>>);
        expect(constant<not wl::dynamic_message_argument<C>>);
        expect(constant<not wl::dynamic_message_argument<D>>);
    };

    wl_tag / "message_payload_size is calculated correctly for short static message"_test = [] {
        constexpr auto message = get_registery{ .registry = {} };

#ifndef __clang__
        // This will crash LLVM 17 and 18 (annoyingly will break clangd), see:
        // https://github.com/llvm/llvm-project/issues/93821#issue-2325716041
        expect(constant<wl::static_message<decltype(message)>>);
#endif

        constexpr auto payload_size  = wl::message_payload_size(message);
        constexpr auto expected_size = sizeof(message.registry);

        expect(constant<expected_size == payload_size>);
    };

    wl_tag / "message_payload_size is calculated correctly for empty static message"_test = [] {
        constexpr auto message = data_src_dtro{};
        expect(constant<std::is_empty_v<decltype(message)>>);

#ifndef __clang__
        // This will crash LLVM 17 and 18 (annoyingly will break clangd), see:
        // https://github.com/llvm/llvm-project/issues/93821#issue-2325716041
        expect(constant<wl::static_message<decltype(message)>>);
#endif

        constexpr auto payload_size  = wl::message_payload_size(message);
        constexpr auto expected_size = 0uz;

        expect(constant<expected_size == payload_size>);
    };

    wl_tag / "message_payload_size is calculated correctly for long static message"_test = [] {
        constexpr auto message = create_buffer{ .id     = { 1 },
                                                .offset = { 2 },
                                                .width  = { 3 },
                                                .height = { 4 },
                                                .stride = { 5 },
                                                .format = wl_shm::format::Egr88 };

#ifndef __clang__
        // This will crash LLVM 17 and 18 (annoyingly will break clangd), see:
        // https://github.com/llvm/llvm-project/issues/93821#issue-2325716041
        expect(constant<wl::static_message<decltype(message)>>);
#endif

        constexpr auto payload_size  = wl::message_payload_size(message);
        constexpr auto expected_size = sizeof(message.id) + sizeof(message.offset)
                                       + sizeof(message.width) + sizeof(message.height)
                                       + sizeof(message.stride) + sizeof(message.format);
        expect(constant<expected_size == payload_size>);
    };
}
