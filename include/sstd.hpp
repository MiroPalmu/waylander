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
/// Supplement library to namespace std.

#include <concepts>
#include <source_location>
#include <type_traits>
#include <utility>

namespace ger {
namespace sstd {

template<typename T>
concept plain_type = std::same_as<std::remove_cvref_t<T>, T>;

template<plain_type T, std::invocable<T&> auto Releaser>
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

/// Throws std::system_error(errno, std::generic_category(), err_msg).
///
/// Where err_msg is quivalent to:
///
/// std::format(
///     "{}:{}:{}: failed with error: {}",
///     loc.file_name(),
///     loc.function_name(),
///     loc.line(),
///     std::strerror(errno)
/// )
[[noreturn]] void
    throw_generic_system_error(const std::source_location loc = std::source_location::current());

/// Throws std::system_error(errno, std::generic_category(), err_msg).
///
/// Where err_msg is quivalent to:
///
/// std::format(
///     "{} failed and transfered {} out of {} total bytes:\n{}",
///     loc.function_name(),
///     transfered_bytes,
///     total_bytes,
///     std::strerror(errno)
/// )
[[noreturn]] void
    throw_partial_system_io_error(const std::size_t transfered_bytes,
                                  const std::size_t total_bytes,
                                  const std::source_location loc = std::source_location::current());

/// Returns dx such that x + dx is multiple of N and dx < N.
template<std::integral auto N>
constexpr auto round_upto_multiple_of(const std::integral auto x) {
    return (N - (x % N)) % N;
};

} // namespace sstd
} // namespace ger
