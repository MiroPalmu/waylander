#pragma once

/// @file
/// Implements storage of overloads/listeners/callbacks/... for messages.

#include <concepts>
#include <cstddef>
#include <functional>
#include <optional>
#include <span>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "wayland/message_intrperter.hpp"
#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {

/// A tool to interpert parsed_message as a message type and pass it to correct overload (callback).
///
/// When interperting raw bytes from parsed_message arguments as a Wayland primitives,
/// the spesific message type has to be inferred based on the object id and opcode.
/// To do this the corresponding interface for the object id has to be stored.
///
/// This type tries to do that with increased type safety.
///
/// Note that the overloads can hold a state, so to make the implementation simpler,
/// a const message_overload_set is not supported.
class message_overload_set {
    using key_t = std::pair<Wobject<generic_object>, Wopcode<generic_object>>;

    struct key_t_hash {
        std::size_t operator()(const key_t& key) const noexcept;
    };

    /// The overloads have different call signatures, so they have to be type erased.
    using erased_overload_t = std::move_only_function<void(std::span<const std::byte>)>;

    std::unordered_map<key_t, erased_overload_t, key_t_hash> overloads_{};

  public:
    template<typename Msg, interface W>
    void add_overload(const Wobject<W> obj_id, std::invocable<Msg> auto&& overload_arg) {
        const auto [_, insert_ok] =
            overloads_.insert({ key_t{ obj_id.value, Msg::opcode.value },
                                [overload = std::forward<decltype(overload_arg)>(overload_arg)](
                                    const std::span<const std::byte> payload) mutable {
                                    std::invoke(overload, interpert_message_payload<Msg>(payload));
                                } });

        if (not insert_ok) {
            throw std::runtime_error{ "Overload for {Wobject<W>, Msg::opcode} already present!" };
        }
    }

    /// Finds overload corresponding to {object id, opcode}-pair or returns empty optional.
    auto overload_resolution(const Wobject<generic_object> obj_id,
                             const Wopcode<generic_object> opcode)
        -> std::optional<std::reference_wrapper<erased_overload_t>>;
};

} // namespace wl
} // namespace ger
