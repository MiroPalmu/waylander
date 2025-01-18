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
/// Implements logic to interpert Wayland wire format payloads.

#include <cstddef>
#include <cstring>
#include <span>
#include <tuple>
#include <utility>

#include "waylander/sstd.hpp"
#include "waylander/type_utils.hpp"
#include "waylander/wayland/message_utils.hpp"
#include "waylander/wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

/// Interperts given bytes as Wayland wire format message payload.
template<typename Msg>
constexpr auto interpert_message_payload(const std::span<const std::byte> payload) -> Msg {
    using msg_primitives_as_tuple           = decltype(sstd::to_tuple(std::declval<Msg>()));
    constexpr auto amount_of_msg_primitives = std::tuple_size_v<msg_primitives_as_tuple>;

    auto bytes_read{ 0uz };

    const auto read_element_32aligned = [&]<typename T>(std::type_identity<T>) -> T {
        if constexpr (std::same_as<Wstring, T>) {
            // Read size:
            Wstring::size_type str_size;
            std::memcpy(&str_size, std::addressof(payload[bytes_read]), sizeof(str_size));

            // Handle null string because if not here then (*) below is broken.
            if (str_size == 0) {
                // null-string
                return Wstring{};
            }

            const auto str_begin = std::addressof(payload[bytes_read + sizeof(Wstring::size_type)]);

            // Increment bytes_read:
            const auto unpadded_total_size = sizeof(str_size) + str_size;
            const auto padded_total_size =
                unpadded_total_size + sstd::round_upto_multiple_of<4>(unpadded_total_size);
            bytes_read += padded_total_size;

            static_assert(std::alignment_of_v<char8_t> == 1, "Sanity check for obscure platforms.");

            return Wstring{ std::u8string_view(
#ifdef __cpp_lib_start_lifetime_as // Techincally UB without this.
                std::start_lifetime_as_array<char8_t>(str_begin, str_size),
#else
                reinterpret_cast<char8_t const*>(str_begin),
#endif
                str_size - 1) }; // (*) -1 for null delimiter
        } else if constexpr (std::same_as<Warray, T>) {
            // Read size:
            Warray::size_type array_size;
            std::memcpy(&array_size, std::addressof(payload[bytes_read]), sizeof(array_size));

            const auto array_begin =
                std::addressof(payload[bytes_read + sizeof(Warray::size_type)]);

            // Increment bytes_read:
            const auto unpadded_total_size = sizeof(array_size) + array_size;
            const auto padded_total_size =
                unpadded_total_size + sstd::round_upto_multiple_of<4>(unpadded_total_size);
            bytes_read += padded_total_size;

            return Warray{ std::span(array_begin, array_size) };

        } else if constexpr (static_message_argument<T>) {
            constexpr auto pad          = sstd::round_upto_multiple_of<4>(sizeof(T));
            constexpr auto element_size = sizeof(T) + pad;
            T elem;
            std::memcpy(std::addressof(elem), std::addressof(payload[bytes_read]), sizeof(T));
            bytes_read += element_size;
            return elem;
        }
    };

    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return Msg{ read_element_32aligned(
            std::type_identity<std::tuple_element_t<I, msg_primitives_as_tuple>>{})... };
    }(std::make_index_sequence<amount_of_msg_primitives>());
}

} // namespace wl
} // namespace ger
