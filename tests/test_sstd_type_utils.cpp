#include <boost/ut.hpp> // import boost.ut;

#include <string_view>
#include <tuple>
#include <type_traits>

#include "type_utils.hpp"

/// Note that nullary template template parameters are impossible, see sstd::arity_of_template_is.
template<typename>
struct nullary_template {};
template<>
struct nullary_template<int> {};

template<typename>
struct unary_template {};
template<typename, typename>
struct binary_template {};
template<typename, typename, typename>
struct ternary_template {};

int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "sstd" } };

    tag("sstd") / "numeral_type<N> works as expected for usual cases"_test = [] {
        using one_int_A        = sstd::numeral_t<1>;
        using one_int_B        = sstd::numeral_t<1, int>;
        using one_int_C        = sstd::numeral_t<1uz, int>;
        using one_int_expected = std::integral_constant<int, 1>;

        expect(constant<std::same_as<one_int_A, one_int_expected>>);
        expect(constant<std::same_as<one_int_B, one_int_expected>>);
        expect(constant<std::same_as<one_int_C, one_int_expected>>);

        using four_long_A        = sstd::numeral_t<4l>;
        using four_long_B        = sstd::numeral_t<4l, long>;
        using four_long_C        = sstd::numeral_t<4, long>;
        using four_long_expected = std::integral_constant<long, 4>;

        expect(constant<std::same_as<four_long_A, four_long_expected>>);
        expect(constant<std::same_as<four_long_B, four_long_expected>>);
        expect(constant<std::same_as<four_long_C, four_long_expected>>);

        using six_size_t_A        = sstd::numeral_t<6uz>;
        using six_size_t_B        = sstd::numeral_t<6uz, std::size_t>;
        using six_size_t_C        = sstd::numeral_t<6z, std::size_t>;
        using six_size_t_expected = std::integral_constant<std::size_t, 6>;

        expect(constant<std::same_as<six_size_t_A, six_size_t_expected>>);
        expect(constant<std::same_as<six_size_t_B, six_size_t_expected>>);
        expect(constant<std::same_as<six_size_t_C, six_size_t_expected>>);
    };

    tag("sstd")
        / "sstd::predicate_type detects if type is derived from std::bool_constant"_test = [] {
        struct true_trait : std::bool_constant<true> {};
        struct false_trait : std::bool_constant<false> {};
        expect(constant<sstd::predicate_type<true_trait>>);
        expect(constant<sstd::predicate_type<false_trait>>);

        struct empty {};
        struct non_trait : std::integral_constant<int, 42> {};
        expect(constant<not sstd::predicate_type<empty>>);
        expect(constant<not sstd::predicate_type<non_trait>>);
    };

    tag("sstd") / "sstd::arity_of_template_is is correct for arities less than 3"_test = [] {
        // Is nullary template?
        expect(constant<not sstd::arity_of_template_is<0, nullary_template>>);
        expect(constant<not sstd::arity_of_template_is<0, unary_template>>);
        expect(constant<not sstd::arity_of_template_is<0, binary_template>>);
        expect(constant<not sstd::arity_of_template_is<0, ternary_template>>);

        // Is unary template?
        //
        // Note that the next expect(...) shows the impossibility
        // of nullary template template parameters.
        // See sstd::arity_of_template_is for more information.
        expect(constant<sstd::arity_of_template_is<1, nullary_template>>);
        expect(constant<sstd::arity_of_template_is<1, unary_template>>);
        expect(constant<not sstd::arity_of_template_is<1, binary_template>>);
        expect(constant<not sstd::arity_of_template_is<1, ternary_template>>);

        // Is binary template?
        expect(constant<not sstd::arity_of_template_is<2, nullary_template>>);
        expect(constant<not sstd::arity_of_template_is<2, unary_template>>);
        expect(constant<sstd::arity_of_template_is<2, binary_template>>);
        expect(constant<not sstd::arity_of_template_is<2, ternary_template>>);

        // Is ternary template?
        expect(constant<not sstd::arity_of_template_is<3, nullary_template>>);
        expect(constant<not sstd::arity_of_template_is<3, unary_template>>);
        expect(constant<not sstd::arity_of_template_is<3, binary_template>>);
        expect(constant<sstd::arity_of_template_is<3, ternary_template>>);
    };

    tag("sstd") / "template_invocable is correct for sample set of standard traits"_test = [] {
        // Unary predicate standard traits
        expect(constant<not sstd::template_invocable<std::is_void>>);
        expect(constant<sstd::template_invocable<std::is_void, int>>);
        expect(constant<not sstd::template_invocable<std::is_void, int, int>>);
        expect(constant<not sstd::template_invocable<std::is_void, int, int, int>>);

        // Binary predicate standard traits
        expect(constant<not sstd::template_invocable<std::is_swappable_with>>);
        expect(constant<not sstd::template_invocable<std::is_swappable_with, int>>);
        expect(constant<sstd::template_invocable<std::is_swappable_with, int, int>>);
        expect(constant<not sstd::template_invocable<std::is_swappable_with, int, int, int>>);

        // N-ary predicate standard traits
        expect(not constant<sstd::template_invocable<std::is_constructible>>);
        expect(constant<sstd::template_invocable<std::is_constructible, int>>);
        expect(constant<sstd::template_invocable<std::is_constructible, int, int>>);
        expect(constant<sstd::template_invocable<std::is_constructible, int, int, int>>);

        // Unary non-predicate standard traits
        expect(constant<not sstd::template_invocable<std::rank>>);
        expect(constant<sstd::template_invocable<std::rank, int>>);
        expect(constant<not sstd::template_invocable<std::rank, int, int>>);
        expect(constant<not sstd::template_invocable<std::rank, int, int, int>>);

        // Binary non-predicate standard traits
        expect(not constant<sstd::template_invocable<std::is_swappable_with>>);
        expect(not constant<sstd::template_invocable<std::is_swappable_with, int>>);
        expect(constant<sstd::template_invocable<std::is_swappable_with, int, int>>);
        expect(not constant<sstd::template_invocable<std::is_swappable_with, int, int, int>>);

        // N-ary non-predicate standard traits
        expect(constant<sstd::template_invocable<std::common_type>>);
        expect(constant<sstd::template_invocable<std::common_type, int>>);
        expect(constant<sstd::template_invocable<std::common_type, int, int>>);
        expect(constant<sstd::template_invocable<std::common_type, int, int, int>>);

        // Miscellaneous predicate standard traits
        expect(constant<sstd::template_invocable<std::conjunction>>);
        expect(constant<sstd::template_invocable<std::conjunction, int>>);
        expect(constant<sstd::template_invocable<std::conjunction, int, int>>);
        expect(constant<sstd::template_invocable<std::conjunction, int, int, int>>);
    };

    tag("sstd") / "template_invoke is correct for sample set of standard traits"_test = [] {
        // Unary predicate standard traits
        expect(
            constant<std::same_as<sstd::template_invoke_t<std::is_void, int>, std::is_void<int>>>);

        // Binary predicate standard traits
        expect(constant<std::same_as<sstd::template_invoke_t<std::is_swappable_with, int, int>,
                                     std::is_swappable_with<int, int>>>);

        // N-ary predicate standard traits
        expect(constant<std::same_as<sstd::template_invoke_t<std::is_constructible, int>,
                                     std::is_constructible<int>>>);
        expect(constant<std::same_as<sstd::template_invoke_t<std::is_constructible, int, int>,
                                     std::is_constructible<int, int>>>);
        expect(constant<std::same_as<sstd::template_invoke_t<std::is_constructible, int, int, int>,
                                     std::is_constructible<int, int, int>>>);

        // Unary non-predicate standard traits
        expect(constant<std::same_as<sstd::template_invoke_t<std::rank, int>, std::rank<int>>>);

        // Binary non-predicate standard traits
        expect(constant<std::same_as<sstd::template_invoke_t<std::is_swappable_with, int, int>,
                                     std::is_swappable_with<int, int>>>);

        // N-ary non-predicate standard traits
        expect(
            constant<std::same_as<sstd::template_invoke_t<std::common_type>, std::common_type<>>>);
        expect(constant<std::same_as<sstd::template_invoke_t<std::common_type, int>,
                                     std::common_type<int>>>);
        expect(constant<std::same_as<sstd::template_invoke_t<std::common_type, int, int>,
                                     std::common_type<int, int>>>);
        expect(constant<std::same_as<sstd::template_invoke_t<std::common_type, int, int, int>,
                                     std::common_type<int, int, int>>>);

        // Miscellaneous predicate standard traits
        expect(
            constant<std::same_as<sstd::template_invoke_t<std::conjunction>, std::conjunction<>>>);
        expect(constant<std::same_as<sstd::template_invoke_t<std::conjunction, int>,
                                     std::conjunction<int>>>);
        expect(constant<std::same_as<sstd::template_invoke_t<std::conjunction, int, int>,
                                     std::conjunction<int, int>>>);
        expect(constant<std::same_as<sstd::template_invoke_t<std::conjunction, int, int, int>,
                                     std::conjunction<int, int, int>>>);
    };

    tag("sstd")
        / "template_invoke_with_repeated_argument is correct for sample set of standard traits"_test =
        [] {
            // Unary predicate standard traits
            expect(constant<std::same_as<
                       sstd::template_invoke_with_repeated_argument_t<std::is_void, int, 1>,
                       std::is_void<int>>>);

            // Binary predicate standard traits
            expect(
                constant<std::same_as<
                    sstd::template_invoke_with_repeated_argument_t<std::is_swappable_with, int, 2>,
                    std::is_swappable_with<int, int>>>);

            // N-ary predicate standard traits
            expect(
                constant<std::same_as<
                    sstd::template_invoke_with_repeated_argument_t<std::is_constructible, int, 1>,
                    std::is_constructible<int>>>);
            expect(
                constant<std::same_as<
                    sstd::template_invoke_with_repeated_argument_t<std::is_constructible, int, 2>,
                    std::is_constructible<int, int>>>);
            expect(
                constant<std::same_as<
                    sstd::template_invoke_with_repeated_argument_t<std::is_constructible, int, 3>,
                    std::is_constructible<int, int, int>>>);

            // Unary non-predicate standard traits
            expect(constant<
                   std::same_as<sstd::template_invoke_with_repeated_argument_t<std::rank, int, 1>,
                                std::rank<int>>>);

            // Binary non-predicate standard traits
            expect(
                constant<std::same_as<
                    sstd::template_invoke_with_repeated_argument_t<std::is_swappable_with, int, 2>,
                    std::is_swappable_with<int, int>>>);

            // N-ary non-predicate standard traits
            expect(constant<std::same_as<
                       sstd::template_invoke_with_repeated_argument_t<std::common_type, int, 0>,
                       std::common_type<>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_repeated_argument_t<std::common_type, int, 1>,
                       std::common_type<int>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_repeated_argument_t<std::common_type, int, 2>,
                       std::common_type<int, int>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_repeated_argument_t<std::common_type, int, 3>,
                       std::common_type<int, int, int>>>);

            // Miscellaneous predicate standard traits
            expect(constant<std::same_as<
                       sstd::template_invoke_with_repeated_argument_t<std::conjunction, int, 0>,
                       std::conjunction<>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_repeated_argument_t<std::conjunction, int, 1>,
                       std::conjunction<int>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_repeated_argument_t<std::conjunction, int, 2>,
                       std::conjunction<int, int>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_repeated_argument_t<std::conjunction, int, 3>,
                       std::conjunction<int, int, int>>>);
        };

    tag("sstd") / "steal_template_args can turn std::tuple<...> to list_type<...>"_test = [] {
        using tuple    = std::tuple<int, float, double>;
        using list     = sstd::steal_template_args_t<tuple, sstd::type_list>;
        using expected = sstd::type_list<int, float, double>;

        expect(constant<std::same_as<list, expected>>);
    };

    tag("sstd") / "unary_predicate_trait_fold is is usable with std::is_floating_point"_test = [] {
        expect(sstd::unary_predicate_trait_fold<std::is_floating_point, float, double, double>);
        expect(not sstd::unary_predicate_trait_fold<std::is_floating_point, int, double, double>);
        expect(not sstd::unary_predicate_trait_fold<std::is_floating_point, float, uint, double>);
        expect(not sstd::unary_predicate_trait_fold<std::is_floating_point, float, double, long>);
    };

    tag("sstd") / "unary_predicate_trait_fold with empty list of types is true"_test = [] {
        expect(sstd::unary_predicate_trait_fold<std::is_enum>);
        expect(sstd::unary_predicate_trait_fold<std::is_floating_point>);

        expect(not sstd::unary_predicate_trait_fold<std::is_enum, int>);
        expect(not sstd::unary_predicate_trait_fold<std::is_floating_point, int>);

        enum foo {};
        expect(sstd::unary_predicate_trait_fold<std::is_enum, foo, foo>);
        expect(sstd::unary_predicate_trait_fold<std::is_floating_point, float, float>);
    };
}
