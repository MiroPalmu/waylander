#pragma once

/// @file Implements container of Wayland protocol messages.

#include <cstddef>
#include <cstring>
#include <ranges>
#include <tuple>
#include <utility>

#include "byte_vec.hpp"
#include "type_utils.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

class message_buffer {
    sstd::byte_vec buff_{};

  public:
    /// True if buffer does not contains any data.
    constexpr bool empty() { return buff_.empty(); }

    template<interface WObj, message_for_inteface<WObj> Message>
    constexpr void append(const Wobject<WObj> obj, const Message& msg) {
        const auto msg_primitives               = sstd::to_ref_tuple(msg);
        constexpr auto amount_of_msg_primitives = std::tuple_size_v<decltype(msg_primitives)>;

        auto msg_total_size = sizeof(message_header<WObj>);

        const auto write_element_32aligned = [&]<typename P>(const P& p) {
            constexpr auto pad_32       = (4 - (sizeof(P) % 4)) % 4;
            constexpr auto element_size = sizeof(P) + pad_32;

            const auto buff_size_before = std::ranges::size(buff_);
            buff_.resize(buff_size_before + element_size);
            std::memcpy(std::addressof(buff_[buff_size_before]), &p, sizeof(P));
            msg_total_size += element_size;
        };

        const auto begin_of_header_index = std::ranges::size(buff_);
        // Have room for the header.
        buff_.resize(buff_.size() + sizeof(message_header<WObj>));

        // Loop over all msg primitives.
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            (write_element_32aligned(std::get<I>(msg_primitives)), ...);
        }(std::make_index_sequence<amount_of_msg_primitives>());

        using underlying_size_t = sstd::underlying_integral_t<Wmessage_size_t>;
        const auto header       = message_header<WObj>{ obj,
                                                        static_cast<underlying_size_t>(msg_total_size),
                                                        Message::opcode };

        std::memcpy(std::addressof(buff_[begin_of_header_index]),
                    std::addressof(header),
                    sizeof(header));
    }

    constexpr auto release_data() -> sstd::byte_vec {
        return std::exchange(buff_, sstd::byte_vec{});
    };
};

} // namespace wl
} // namespace ger
