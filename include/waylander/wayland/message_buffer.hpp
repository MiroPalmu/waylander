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

#pragma once

/// @file
/// Implements container of Wayland protocol messages.

#include <cassert>
#include <cstddef>
#include <cstring>
#include <ranges>
#include <tuple>
#include <utility>
#include <vector>

#include "waylander/byte_vec.hpp"
#include "waylander/sstd.hpp"
#include "waylander/type_utils.hpp"
#include "waylander/wayland/protocol_primitives.hpp"

namespace waylander {
namespace wl {

class message_buffer {
    sstd::byte_vec buff_{};
    std::vector<Wfd> fd_buff_{};

  public:
    /// True if buffer does not contains any data.
    constexpr bool empty() { return buff_.empty() and fd_buff_.empty(); }

    template<interface WObj, message_for_inteface<WObj> Message>
    constexpr void append(const Wobject<WObj> obj, const Message& msg) {
        const auto msg_primitives               = sstd::to_ref_tuple(msg);
        constexpr auto amount_of_msg_primitives = std::tuple_size_v<decltype(msg_primitives)>;

        auto msg_total_size = sizeof(message_header<WObj>);

        const auto write_element_32aligned = sstd::overloaded{
            [&](const Wstring& str) {
                const auto length_of_str            = str.size() + 1uz; // + 1 for \0 delimiter.
                const auto element_size_without_pad = sizeof(Wstring::size_type) + length_of_str;

                const auto element_size =
                    element_size_without_pad
                    + sstd::round_upto_multiple_of<4>(element_size_without_pad);

                const auto buff_size_before = std::ranges::size(buff_);
                buff_.resize(buff_size_before + element_size);

                // size
                assert(std::in_range<Wstring::size_type>(length_of_str));
                const auto length_of_str_correct_type =
                    static_cast<Wstring::size_type>(length_of_str);
                std::memcpy(std::addressof(buff_[buff_size_before]),
                            std::addressof(length_of_str_correct_type),
                            sizeof(Wstring::size_type));
                // string
                std::memcpy(std::addressof(buff_[buff_size_before + sizeof(Wstring::size_type)]),
                            str.data(),
                            str.size());

                // null delimiter
                buff_[buff_size_before + element_size_without_pad - 1uz] = std::byte{ 0 };

                msg_total_size += element_size;
            },
            [&](const Warray& arr) {
                const auto element_size_without_pad = sizeof(Warray::size_type) + arr.size();
                const auto element_size =
                    element_size_without_pad
                    + sstd::round_upto_multiple_of<4>(element_size_without_pad);

                //const auto element_size_without_pad =

                const auto buff_size_before = std::ranges::size(buff_);
                buff_.resize(buff_size_before + element_size);

                // size
                assert(std::in_range<Warray::size_type>(arr.size()));
                const auto array_size_correct_type = static_cast<Warray::size_type>(arr.size());
                std::memcpy(std::addressof(buff_[buff_size_before]),
                            std::addressof(array_size_correct_type),
                            sizeof(array_size_correct_type));
                // array
                std::memcpy(std::addressof(buff_[buff_size_before + sizeof(Wstring::size_type)]),
                            arr.data(),
                            arr.size());
                msg_total_size += element_size;
            },
            [&](const Wfd& fd) { fd_buff_.push_back(fd); },
            [&]<typename P>(const P& p) {
                constexpr auto pad          = sstd::round_upto_multiple_of<4>(sizeof(P));
                constexpr auto element_size = sizeof(P) + pad;

                const auto buff_size_before = std::ranges::size(buff_);
                buff_.resize(buff_size_before + element_size);
                std::memcpy(std::addressof(buff_[buff_size_before]), &p, sizeof(P));
                msg_total_size += element_size;
            }
        };

        const auto begin_of_header_index = std::ranges::size(buff_);
        // Have room for the header.
        buff_.resize(buff_.size() + sizeof(message_header<WObj>));

        // Loop over all msg primitives.
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            (write_element_32aligned(std::get<I>(msg_primitives)), ...);
        }(std::make_index_sequence<amount_of_msg_primitives>());

        const auto narrowed_msg_size = static_cast<Wmessage_size_t::integral_type>(msg_total_size);
        const auto header = message_header<WObj>(obj, Message::opcode, { narrowed_msg_size });

        std::memcpy(std::addressof(buff_[begin_of_header_index]),
                    std::addressof(header),
                    sizeof(header));
    }

    constexpr auto release_data() -> sstd::byte_vec {
        return std::exchange(buff_, sstd::byte_vec{});
    };

    constexpr auto release_fds() -> std::vector<Wfd> {
        return std::exchange(fd_buff_, std::vector<Wfd>{});
    };
};

} // namespace wl
} // namespace waylander
