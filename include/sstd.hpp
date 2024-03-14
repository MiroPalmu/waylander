#pragma once

/// @file supplement to namespace std

#include <concepts>
#include <type_traits>
#include <utility>

namespace ger {
namespace sstd {

template<typename T>
concept potentially_owning_legacy_resource = std::same_as<std::remove_cvref_t<T>, int>;

template<typename Handler, typename F, typename... Args>
concept handled_legacy_constructor =
    std::invocable<F, Args...>
    and potentially_owning_legacy_resource<std::invoke_result_t<F, Args...>>
    and std::constructible_from<Handler, std::invoke_result_t<F, Args...>>;

template<typename Handler, typename F, typename... Args>
    requires handled_legacy_constructor<Handler, F, Args...>
auto legacy_construct(F&& legacy_constructor, Args&&... args) -> Handler {
    return std::forward<F>(legacy_constructor)(std::forward<Args>(args)...);
}

template<typename I, typename J>
concept same_signess_integrals = (std::signed_integral<I> and std::signed_integral<J>)
                                 or (std::unsigned_integral<I> and std::unsigned_integral<J>);

template<std::integral I>
struct integral_like {
    I value;
    using integral_type = I;

    template<std::integral J>
        requires same_signess_integrals<I, J>
    [[nodiscard]] constexpr bool operator==(this auto&& self, const J rhs) {
        return self.value == rhs;
    }
};

} // namespace sstd
} // namespace ger
