#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace ger {
namespace sstd {

/// Instantiates template template parameters to get infromation about the template template.
struct probe_type {};

/// Pairs type \p T to \p Index. Useful as indexed_type<I, T>::type... where I is parameter pack.
template<std::size_t Index, typename T>
struct indexed_type : std::integral_constant<std::size_t, Index> {
    using type = T;
};
/// Helper for indexed_type.
template<std::size_t Index, typename T>
using indexed_type_t = indexed_type<Index, T>::type;

/// Shorthand for std::integral_constant.
template<std::integral auto N, std::integral T = decltype(N)>
using numeral_t = std::integral_constant<T, N>;

/// Template \p TT has arity of \p N, when T<probe_type...> is valid type for \p N probe_types.
///
/// Arity of a template template parameter can never be 0,
/// because they are possible only with partial specializations and
/// template template parameters only match the primary template, not the partial specializations.
///
/// This is not perfect test, but enables sligthly better errors in some situations.
///
template<std::size_t N, template<typename...> typename TT>
concept arity_of_template_is = ((N > 0)and[]<std::size_t... I>(std::index_sequence<I...>) {
    return requires { typename TT<indexed_type_t<I, probe_type>...>; };
}(std::make_index_sequence<N>{}));

/// "Invoking" template \p TT using pack T..., i.e. TT<T...>, is valid.
template<template<typename...> typename TT, typename... T>
concept template_invocable = requires { typename TT<T...>; };

/// "Invoke" template \p TT using pack T..., i.e. TT<T...>.
///
/// Just more verbose TT<T...> with better error messages thanks to the constraint.
template<template<typename...> typename TT, typename... T>
    requires template_invocable<TT, T...>
struct template_invoke {
    using type = TT<T...>;
};
/// Helper for template_invoke.
template<template<typename...> typename TT, typename... T>
    requires template_invocable<TT, T...>
using template_invoke_t = template_invoke<TT, T...>::type;

/// "Invoking" template \p TT using \p N amount of \p T is valid.
template<template<typename...> typename TT, typename T, std::size_t N>
concept template_invocable_with_repeated_argument =
    ([]<std::size_t... I>(std::index_sequence<I...>) {
        return template_invocable<TT, indexed_type_t<I, T>...>;
    }(std::make_index_sequence<N>{}));

/// "Invoke" template \p TT using \p N amount of \p T.
template<template<typename...> typename TT, typename T, std::size_t N>
    requires template_invocable_with_repeated_argument<TT, T, N>
struct template_invoke_with_repeated_argument {
    using type = decltype([]<std::size_t... I>(std::index_sequence<I...>) {
        return std::type_identity<template_invoke_t<TT, indexed_type_t<I, T>...>>{};
    }(std::make_index_sequence<N>{}))::type;
};
/// Helper for template_invoke_with_repeated_argument.
template<template<typename...> typename TT, typename T, std::size_t N>
    requires template_invocable_with_repeated_argument<TT, T, N>
using template_invoke_with_repeated_argument_t =
    template_invoke_with_repeated_argument<TT, T, N>::type;

/// See named requiremend [Unary|Binary]TypeTrait
template<typename T>
concept predicate_type = (std::derived_from<T, std::bool_constant<true>>
                          or std::derived_from<T, std::bool_constant<false>>);

template<std::size_t N, template<typename...> typename Trait>
concept nary_predicate_trait =
    template_invocable_with_repeated_argument<Trait, probe_type, N>
    and predicate_type<template_invoke_with_repeated_argument_t<Trait, probe_type, N>>;

template<template<typename> typename Trait>
concept unary_predicate_trait = std::derived_from<Trait<probe_type>, std::bool_constant<true>>
                                or std::derived_from<Trait<probe_type>, std::bool_constant<false>>;

/// Satisfied if type trait Trait is satisfied for all types T... .
template<template<typename> typename Trait, typename... T>
concept unary_predicate_trait_fold = unary_predicate_trait<Trait> and (Trait<T>::value && ...);

/// Helper for visitor patter to create overloaded function objects.
template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

/// Conditionally turns type \p f to std::function<f> to make overloadable work with function types.
template<typename f>
using make_overloadable = std::conditional_t<std::is_function_v<f>, std::function<f>, f>;

/// Helper to make overloaded<...> accept function types in addition to other callables.
template<typename... F>
using make_overloaded = overloaded<make_overloadable<F>...>;

template<typename... F>
struct type_map {
    template<typename f>
    using make_overloadable = std::conditional_t<std::is_function_v<f>, std::function<f>, f>;

    using visitor = overloaded<make_overloadable<F>...>;

    template<typename... T>
    using type = decltype(std::declval<visitor>()(std::declval<T>()...));
};

template<typename... T>
struct type_list {
    template<typename... F>
    using map = type_list<typename type_map<F...>::template type<T>...>;
};

/// Primary template for type trait to check if T == type_list.
template<typename>
struct is_type_list : std::integral_constant<bool, false> {};
/// Partial specialization of is_type_list.
template<typename... T>
struct is_type_list<type_list<T...>> : std::integral_constant<bool, true> {};

/// Helper for is_type_list
template<typename T>
inline constexpr bool is_type_list_v = is_type_list<T>::value;

template<template<typename...> typename TT, typename T>
    requires(is_type_list_v<T>)
struct is_template_invocable_with_list;

/// Type trait implementation of concept template_invocable_with_list.
template<template<typename...> typename TT, typename... T>
struct is_template_invocable_with_list<TT, type_list<T...>> :
    std::integral_constant<bool, template_invocable<TT, T...>> {};

template<template<typename...> typename TT, typename T>
    requires(is_type_list_v<T>)
inline constexpr bool is_template_invocable_with_list_v =
    is_template_invocable_with_list<TT, T>::value;

/// "Invoking" template \p TT using pack T... from type_list<T...>, i.e. TT<T...>, is valid.
template<template<typename...> typename TT, typename List>
concept template_invocable_with_list = (is_template_invocable_with_list_v<TT, List>);

/// "Invoke" template \p TT using pack T... form type_list<T...>, i.e. TT<T...>.
///
/// Primary template.
template<template<typename...> typename TT, typename List>
    requires template_invocable_with_list<TT, List>
struct template_invoke_with_list;

/// Partial specialization for template_invoke_with_list.
template<template<typename...> typename TT, typename... T>
struct template_invoke_with_list<TT, type_list<T...>> {
    using type = template_invoke_t<TT, T...>;
};

/// Helper for template_invoke_with_list;
template<template<typename...> typename TT, typename List>
    requires template_invocable_with_list<TT, List>
using template_invoke_with_list_t = typename template_invoke_with_list<TT, List>::type;

/// Primary template for the single specialization.
template<typename T, template<typename...> typename TT>
struct steal_template_args;

/// Turns TT1<T...> to TT2<T...>.
template<template<typename...> typename TT1, template<typename...> typename TT2, typename... T>
struct steal_template_args<TT1<T...>, TT2> {
    using type = TT2<T...>;
};

/// Helper for steal_template_args.
template<typename T, template<typename...> typename TT>
using steal_template_args_t = steal_template_args<T, TT>::type;

/// Implicit converstion to any type T is syntaxically correct but not defined.
struct morph_type {
    template<class T>
    constexpr operator T(); // non explicit
};

/// Returns std::tuple<const T&...> where T... are the elements of \p t.
template<typename T>
    requires(std::is_aggregate_v<std::remove_cvref_t<T>>)
constexpr auto to_ref_tuple(const T& t) {
    if constexpr (std::constructible_from<T,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type>) {
        const auto& [x20,
                     x19,
                     x18,
                     x17,
                     x16,
                     x15,
                     x14,
                     x13,
                     x12,
                     x11,
                     x10,
                     x9,
                     x8,
                     x7,
                     x6,
                     x5,
                     x4,
                     x3,
                     x2,
                     x1] = t;
        return std::make_tuple(std::cref(x20),
                               std::cref(x19),
                               std::cref(x18),
                               std::cref(x17),
                               std::cref(x16),
                               std::cref(x15),
                               std::cref(x14),
                               std::cref(x13),
                               std::cref(x12),
                               std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x19,
                     x18,
                     x17,
                     x16,
                     x15,
                     x14,
                     x13,
                     x12,
                     x11,
                     x10,
                     x9,
                     x8,
                     x7,
                     x6,
                     x5,
                     x4,
                     x3,
                     x2,
                     x1] = t;
        return std::make_tuple(std::cref(x19),
                               std::cref(x18),
                               std::cref(x17),
                               std::cref(x16),
                               std::cref(x15),
                               std::cref(x14),
                               std::cref(x13),
                               std::cref(x12),
                               std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x18,
                     x17,
                     x16,
                     x15,
                     x14,
                     x13,
                     x12,
                     x11,
                     x10,
                     x9,
                     x8,
                     x7,
                     x6,
                     x5,
                     x4,
                     x3,
                     x2,
                     x1] = t;
        return std::make_tuple(std::cref(x18),
                               std::cref(x17),
                               std::cref(x16),
                               std::cref(x15),
                               std::cref(x14),
                               std::cref(x13),
                               std::cref(x12),
                               std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x17, x16, x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] =
            t;
        return std::make_tuple(std::cref(x17),
                               std::cref(x16),
                               std::cref(x15),
                               std::cref(x14),
                               std::cref(x13),
                               std::cref(x12),
                               std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x16, x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x16),
                               std::cref(x15),
                               std::cref(x14),
                               std::cref(x13),
                               std::cref(x12),
                               std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x15),
                               std::cref(x14),
                               std::cref(x13),
                               std::cref(x12),
                               std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x14),
                               std::cref(x13),
                               std::cref(x12),
                               std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x13),
                               std::cref(x12),
                               std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x12),
                               std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x11),
                               std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x10),
                               std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x9),
                               std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x8),
                               std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x7),
                               std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x6),
                               std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (
        std::constructible_from<T, morph_type, morph_type, morph_type, morph_type, morph_type>) {
        const auto& [x5, x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x5),
                               std::cref(x4),
                               std::cref(x3),
                               std::cref(x2),
                               std::cref(x1));
    } else if constexpr (
        std::constructible_from<T, morph_type, morph_type, morph_type, morph_type>) {
        const auto& [x4, x3, x2, x1] = t;
        return std::make_tuple(std::cref(x4), std::cref(x3), std::cref(x2), std::cref(x1));
    } else if constexpr (std::constructible_from<T, morph_type, morph_type, morph_type>) {
        const auto& [x3, x2, x1] = t;
        return std::make_tuple(std::cref(x3), std::cref(x2), std::cref(x1));
    } else if constexpr (std::constructible_from<T, morph_type, morph_type>) {
        const auto& [x2, x1] = t;
        return std::make_tuple(std::cref(x2), std::cref(x1));
    } else if constexpr (std::constructible_from<T, morph_type>) {
        const auto& [x1] = t;
        return std::make_tuple(std::cref(x1));
    } else {
        static_assert(false, "Waiting room for reflection...");
    }
}

/// Returns copy of \p t as a std::tuple<T...>, where T... are the elements of \p t.
template<typename T>
    requires(std::is_aggregate_v<std::remove_cvref_t<T>>)
constexpr auto to_tuple(const T& t) {
    if constexpr (std::constructible_from<T,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type,
                                          morph_type>) {
        const auto& [x20,
                     x19,
                     x18,
                     x17,
                     x16,
                     x15,
                     x14,
                     x13,
                     x12,
                     x11,
                     x10,
                     x9,
                     x8,
                     x7,
                     x6,
                     x5,
                     x4,
                     x3,
                     x2,
                     x1] = t;
        return std::make_tuple(x20,
                               x19,
                               x18,
                               x17,
                               x16,
                               x15,
                               x14,
                               x13,
                               x12,
                               x11,
                               x10,
                               x9,
                               x8,
                               x7,
                               x6,
                               x5,
                               x4,
                               x3,
                               x2,
                               x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x19,
                     x18,
                     x17,
                     x16,
                     x15,
                     x14,
                     x13,
                     x12,
                     x11,
                     x10,
                     x9,
                     x8,
                     x7,
                     x6,
                     x5,
                     x4,
                     x3,
                     x2,
                     x1] = t;
        return std::make_tuple(x19,
                               x18,
                               x17,
                               x16,
                               x15,
                               x14,
                               x13,
                               x12,
                               x11,
                               x10,
                               x9,
                               x8,
                               x7,
                               x6,
                               x5,
                               x4,
                               x3,
                               x2,
                               x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x18,
                     x17,
                     x16,
                     x15,
                     x14,
                     x13,
                     x12,
                     x11,
                     x10,
                     x9,
                     x8,
                     x7,
                     x6,
                     x5,
                     x4,
                     x3,
                     x2,
                     x1] = t;
        return std::make_tuple(x18,
                               x17,
                               x16,
                               x15,
                               x14,
                               x13,
                               x12,
                               x11,
                               x10,
                               x9,
                               x8,
                               x7,
                               x6,
                               x5,
                               x4,
                               x3,
                               x2,
                               x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x17, x16, x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] =
            t;
        return std::
            make_tuple(x17, x16, x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x16, x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::
            make_tuple(x16, x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x15, x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x14, x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x13, x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x12, x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x11, x10, x9, x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x10, x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x10, x9, x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x9, x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x9, x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x8, x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x8, x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x7, x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x7, x6, x5, x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type,
                                                 morph_type>) {
        const auto& [x6, x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x6, x5, x4, x3, x2, x1);
    } else if constexpr (
        std::constructible_from<T, morph_type, morph_type, morph_type, morph_type, morph_type>) {
        const auto& [x5, x4, x3, x2, x1] = t;
        return std::make_tuple(x5, x4, x3, x2, x1);
    } else if constexpr (
        std::constructible_from<T, morph_type, morph_type, morph_type, morph_type>) {
        const auto& [x4, x3, x2, x1] = t;
        return std::make_tuple(x4, x3, x2, x1);
    } else if constexpr (std::constructible_from<T, morph_type, morph_type, morph_type>) {
        const auto& [x3, x2, x1] = t;
        return std::make_tuple(x3, x2, x1);
    } else if constexpr (std::constructible_from<T, morph_type, morph_type>) {
        const auto& [x2, x1] = t;
        return std::make_tuple(x2, x1);
    } else if constexpr (std::constructible_from<T, morph_type>) {
        const auto& [x1] = t;
        return std::make_tuple(x1);
    } else {
        static_assert(false, "Waiting room for reflection...");
    }
}

} // namespace sstd
} // namespace ger
