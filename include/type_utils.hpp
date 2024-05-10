#pragma once

#include <concepts>
#include <type_traits>

namespace ger {
namespace sstd {

/// Used to instantiate template template parameters to get infromation about the template template.
struct probe_type {};

template<template<typename> typename Trait>
concept unary_predicate_trait = std::derived_from<Trait<probe_type>, std::bool_constant<true>>
                                or std::derived_from<Trait<probe_type>, std::bool_constant<false>>;

/// Satisfied if type trait Trait is satisfied for all types T... .
template<template<typename> typename Trait, typename... T>
concept unary_predicate_trait_fold = unary_predicate_trait<Trait> and (Trait<T>::value && ...);

} // namespace sstd
} // namespace ger
