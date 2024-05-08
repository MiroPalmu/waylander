#include <config.h>

#include <cstddef>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <type_traits>

#include "wayland/message_parser.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

[[nodiscard]] auto message_parser::message_generator() -> std::generator<const parsed_message&> {
    // Points to the beginning of the next unparsed message = NUM.
    auto NUM_begin = unparsed_messages_.begin();

    const auto get_NUM_size = [&]() -> wl::Wmessage_size_t {
#ifdef __cpp_lib_is_implicit_lifetime
        static_assert(std::is_implicit_lifetime_v<wl::Wmessage_size_t>);
#endif
        wl::Wmessage_size_t size;
        const auto ptr_to_size =
            std::ranges::next(NUM_begin, sizeof(wl::Wobject<wl::generic_object>));
        std::memcpy(&size, &*ptr_to_size, sizeof(wl::Wmessage_size_t));

        if (size.value < 8u) {
            throw std::logic_error{ "Wayland message size is less than 8 bytes." };
        }

        return size;
    };

    const auto parse_NUM = [&]() -> parsed_message {
        const auto unparsed_bytes = std::ranges::distance(NUM_begin, unparsed_messages_.end());
        if (unparsed_bytes < 8) {
            throw std::logic_error{
                "Trying to interpert less than 8 bytes as Wayland message header."
            };
        }

        constexpr auto id_size     = sizeof(wl::Wobject<wl::generic_object>);
        constexpr auto size_size   = sizeof(wl::Wmessage_size_t);
        constexpr auto opcode_size = sizeof(wl::Wopcode<wl::generic_object>);
        const auto ptr_to_opcode   = std::ranges::next(NUM_begin, id_size + size_size);

        // Default intialize NUM, meaning that its members:
        //     - object_id
        //     - opcode
        // which are implicit-lifetime types, can be initialized using std::memcpy.

#ifdef __cpp_lib_is_implicit_lifetime
        static_assert(std::is_implicit_lifetime_v<wl::Wobject<wl::generic_object>>);
        static_assert(std::is_implicit_lifetime_v<wl::Wopcode<wl::generic_object>>);
#endif

        parsed_message NUM;
        std::memcpy(&NUM.object_id, &*NUM_begin, id_size);
        std::memcpy(&NUM.opcode, &*ptr_to_opcode, opcode_size);
        return NUM;
    };

    // Message which is being constructed from NUM.
    while (NUM_begin != unparsed_messages_.end()) {
        const auto msg = parse_NUM();
        co_yield msg;
        std::ranges::advance(NUM_begin, get_NUM_size().value, unparsed_messages_.end());
    }
}

} // namespace wl
} // namespace ger
