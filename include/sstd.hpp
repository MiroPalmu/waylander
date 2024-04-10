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

/// Can be (publicly) inherited to integralifie a type.
template<std::integral I>
struct integralifyer {
    I value;
    using integral_type = I;
};

template<typename T>
concept integralified = std::integral<typename T::integral_type>
                        and std::derived_from<T, integralifyer<typename T::integral_type>>;

template<typename T>
concept integral_compatible = std::integral<T> or integralified<T>;

template<typename>
struct underlying_integral;

template<typename T>
    requires integralified<std::remove_cvref_t<T>>
struct underlying_integral<T> {
    using type = typename std::remove_cvref_t<T>::integral_type;
};

template<typename T>
    requires std::integral<std::remove_cvref_t<T>>
struct underlying_integral<T> {
    using type = std::remove_cvref_t<T>;
};

template<typename T>
using underlying_integral_t = typename underlying_integral<T>::type;

template<typename T>
    requires integralified<std::remove_cvref_t<T>>
[[nodiscard]] static auto get_integral_value(T&& x) noexcept -> underlying_integral_t<T> {
    return std::forward<T>(x).value;
}

template<typename T>
    requires std::integral<std::remove_cvref_t<T>>
[[nodiscard]] static auto get_integral_value(const T x) noexcept -> T {
    return x;
}

/// Comparison between integral_compatible types.
///
/// Get value of \p x regrdles if it is real integral or integralified one.
/// Comparison is safe regardless of the underying integral types because
/// usage of std::cmp_equal.
template<typename L, typename R>
    requires integral_compatible<std::remove_cvref_t<L>>
             and integral_compatible<std::remove_cvref_t<R>>
[[nodiscard]] constexpr bool operator==(L&& lhs, R&& rhs) {
    return std::cmp_equal(get_integral_value(std::forward<L>(lhs)),
                          get_integral_value(std::forward<R>(rhs)));
}

template<typename T>
concept plain_type = std::same_as<std::remove_cvref_t<T>, T>;

template<typename T>
concept potentially_owning_legacy_handle =
    plain_type<T> and std::movable<T> and (std::is_trivially_destructible_v<T>);

template<potentially_owning_legacy_handle T, std::invocable<T&> auto Releaser>
class unique_handle : public T {
    /// Moved objects are marked inactive.
    bool active_{ true };

  public:
    /// Constructible only from r-value reference.
    unique_handle(T&& resource) : T{ std::move(resource) } {}

    ~unique_handle() {
        if (active_) {
            active_ = false; // This should only matter if dtor is called explicitly.
            Releaser(static_cast<T&>(*this));
        }
    }

    /// Move constructible.
    ///
    /// Marks resource in \p rhs as inactive and calls move constructor of the base T,
    /// as otherwise the base is default initialized.
    unique_handle(unique_handle&& rhs) noexcept : T{ std::move(rhs) } {
        active_ = std::exchange(rhs.active_, false);
    }

    /// Move assignable.
    ///
    /// *this releases resource if it is currently active,
    /// before calling the move constructor of the base T.
    unique_handle& operator=(unique_handle&& rhs) noexcept {
        this->~unique_handle();
        active_ = std::exchange(rhs.active_, false);
        // Call move assignment of the base.
        this->T::operator=(static_cast<T&&>(rhs));
        return *this;
    }

    /// Unique => not copyable.
    unique_handle(const unique_handle&) = delete;
    /// Unique => not copyable.
    unique_handle& operator=(const unique_handle&) = delete;
};

} // namespace sstd
} // namespace ger
