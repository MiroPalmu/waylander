#include <config.h>

#include <array>
#include <cstring>

#include "wayland/message_overload_set.hpp"

std::size_t ger::wl::message_overload_set::key_t_hash::operator()(const key_t& key) const noexcept {
    using combined_t = std::size_t;
    static_assert(sizeof(combined_t) >= sizeof(key.first) + sizeof(key.second));

    alignas(combined_t) std::array<std::byte, sizeof(combined_t)> combined;
    static_assert(sizeof(combined) == sizeof(combined_t));

    const auto first_at_combined    = combined.begin();
    const auto second_at_combined   = std::ranges::next(combined.begin(), sizeof(key.first));
    const auto leftover_at_combined = std::ranges::next(second_at_combined, sizeof(key.second));

    std::memcpy(first_at_combined, std::addressof(key.first), sizeof(key.first));
    std::memcpy(second_at_combined, std::addressof(key.second), sizeof(key.second));
    std::memset(leftover_at_combined,
                0,
                sizeof(std::size_t) - sizeof(key.first) - sizeof(key.second));

    return std::hash<std::size_t>{}(std::bit_cast<std::size_t>(combined));
}

auto ger::wl::message_overload_set::overload_resolution(const Wobject<generic_object> obj_id,
                                                        const Wopcode<generic_object> opcode)
    -> std::optional<std::reference_wrapper<erased_overload_t>> {
    const auto ov_resolution = overloads_.find({ obj_id, opcode });
    if (ov_resolution == overloads_.end()) {
        return {};
    } else {
        return ov_resolution->second;
    }
}
