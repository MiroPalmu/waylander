#pragma once

/// @file Allocator adaptor to customize how allocator constructs objects (A::construct).
///
/// Below is discussion on std::vector::resize, which is one use case for customizing
/// allocator construction.
///
/// Normally std::vector::resize will append default-inserted [1] elements [2],
/// via std::allocator_traits<A>::construct(A& a, ...) [1], which by default will use
/// std::construct_at [3], if a.construct(...) is not present. This means that
/// the default-inserted elements are value-initialized, by default!
///
/// However value-initialization might not be desired if the elemenets are non-class type
/// and zeroing out is not needed. This can be prevented by overwriting A::construct member
/// function of a allocator.
///
/// This implementation is adopted from [4].
///
/// [1] https://en.cppreference.com/w/cpp/named_req/DefaultInsertable
/// [2] https://en.cppreference.com/w/cpp/container/vector/resize
/// [3] https://en.cppreference.com/w/cpp/memory/allocator_traits/construct
/// [4] https://stackoverflow.com/questions/21028299/is-this-behavior-of-vectorresizesize-type-n-under-c11-and-boost-container/21028912#21028912

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

namespace ger {
namespace sstd {
namespace allocator {

template<typename T>
concept allocator_adapter = requires { typename T::adapted_allocator_traits; };

/// Impelements construct which alwyas constructs the object with copy of \p C.
template<auto C>
struct constant_construct {
    template<typename U, typename... Args>
        requires std::constructible_from<U, decltype(C)>
    constexpr void construct(U* const ptr, Args&&...) {
        ::new (static_cast<void*>(ptr)) U{ C };
    }
};

/// Implements default-initializing construct for construct_allocator_adapter.
struct default_init_construct {
    /// Default-inits (instead of value-init) object of type U at \p ptr with true placement new.
    ///
    /// If arguments are given, delecates the construction to the adapted allocator.
    template<allocator_adapter Adapter, typename U, typename... Args>
        requires(std::default_initializable<U> and sizeof...(Args) == 0)
                or std::constructible_from<U, Args...>
    constexpr void construct(this Adapter& self, U* const ptr, Args&&... args) noexcept(
        std::is_nothrow_default_constructible_v<U> and sizeof...(Args) == 0) {
        if constexpr (sizeof...(Args) == 0) {
            // Must use qualified ::new and cast ptr to void*, to call true placement-new.
            // https://en.cppreference.com/w/cpp/memory/new/operator_new#Notes
            ::new (static_cast<void*>(ptr)) U;
        } else {
            // Delecate the construction to the adapted allocator.
            using adapted_allocator_traits = typename Adapter::adapted_allocator_traits;
            using adapted_allocator        = typename adapted_allocator_traits::allocator_type;
            adapted_allocator_traits::construct(static_cast<adapted_allocator&>(self),
                                                ptr,
                                                std::forward<Args>(args)...);
        }
    }
};

/// Allocator adaptor to customize object construction to allocated storage.
template<typename ConstructImpl, typename T, typename A = std::allocator<T>>
class construct_allocator_adapter : public A, private ConstructImpl {
  public:
    using adapted_allocator_traits = std::allocator_traits<A>;

    template<typename U>
    struct rebind {
        using other = construct_allocator_adapter<
            ConstructImpl,
            U,
            typename adapted_allocator_traits::template rebind_alloc<U>>;
    };

    using A::A;
    using ConstructImpl::construct;
};

} // namespace allocator
} // namespace sstd
} // namespace ger
