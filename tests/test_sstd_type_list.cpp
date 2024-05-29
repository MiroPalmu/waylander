#include <boost/ut.hpp> // import boost.ut;

#include <concepts>
#include <type_traits>

#include "type_utils.hpp"

// short hand
template<typename... T>
using L = ger::sstd::type_list<T...>;

int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "sstd" } };

    // type_list::map<...>:

    tag("sstd") / "type_list::map maps single short to long"_test = [] {
        using single_short = sstd::type_list<short>;
        using mapped       = single_short::map<long(short)>;
        using expected     = sstd::type_list<long>;

        expect(constant<std::same_as<mapped, expected>>);
    };

    tag("sstd") / "type_list::map maps multiple shorts to longs"_test = [] {
        using shorts   = sstd::type_list<short, short, short>;
        using mapped   = shorts::map<long(short)>;
        using expected = sstd::type_list<long, long, long>;

        expect(constant<std::same_as<mapped, expected>>);
    };

    tag("sstd") / "type_list::map maps short to long and vise versa"_test = [] {
        using shorts_and_longs = sstd::type_list<short, long, long, short>;
        using mapped           = shorts_and_longs::map<long(short), short(long)>;
        using expected         = sstd::type_list<long, short, short, long>;

        expect(constant<std::same_as<mapped, expected>>);
    };

    tag("sstd") / "type_list::map on empty list works"_test = [] {
        using empty  = sstd::type_list<>;
        using mapped = empty::map<int(int), float(float)>;

        expect(constant<std::same_as<empty, mapped>>);
    };

    /// Does not compile => can not be tested here :(
    // tag("sstd") / "type_list::map with ambiguous overload is ill-formed"_test = [] {
    //     using empty = sstd::type_list<int>;
    //     expect(not requires { typename empty::map<int(int), unsigned(int)>; });
    // };

    tag("sstd") / "type_list::map works with generic lambdas"_test = [] {
        using list           = sstd::type_list<int, double, float>;
        using generic_lambda = decltype([](auto) -> short { return {}; });
        using mapped         = list::map<unsigned(int), generic_lambda>;
        using expected       = sstd::type_list<unsigned, short, short>;

        expect(constant<std::same_as<expected, mapped>>);
    };

    // type_list::fold_left<...>:

    tag("sstd") / "type_list::fold_left can be used to accumulate integral constants"_test = [] {
        using integrals   = sstd::type_list<sstd::numeral_t<1>,
                                          sstd::numeral_t<42>,
                                          sstd::numeral_t<0>,
                                          sstd::numeral_t<100>>;
        using binary_op   = decltype([](auto Lhs, auto Rhs) {
            return sstd::numeral_t<decltype(Lhs)::value + decltype(Rhs)::value>{};
        });
        using fold_result = integrals::fold_left<sstd::numeral_t<2000>, binary_op>;
        using expected    = sstd::numeral_t<2143>;

        expect(constant<std::same_as<fold_result, expected>>);
    };

    tag("sstd") / "type_list::fold_left can be use overloaded binary operation"_test = [] {
        using integrals   = sstd::type_list<short, int, long>;
        using fold_result = integrals::fold_left<unsigned short,
                                                 short(unsigned short, short),
                                                 unsigned(short, int),
                                                 long long(unsigned, long)>;

        expect(constant<std::same_as<long long, fold_result>>);
    };

    tag("sstd") / "type_list::fold_left<Init, ...> is a noop for empty list"_test = [] {
        using empty       = sstd::type_list<>;
        using fold_result = empty::fold_left<double, int(int, int), long(double, double)>;

        expect(constant<std::same_as<double, fold_result>>);
    };

    tag("sstd") / "type_list::fold_left<Init, ...> works for list of size one"_test = [] {
        using empty       = sstd::type_list<long>;
        using fold_result = empty::fold_left<short, int(short, long), unsigned(long, short)>;

        expect(constant<std::same_as<int, fold_result>>);
    };

    // type_list<T...>::append<U...>:

    tag("sstd") / "type_list can be build with type_list::append"_test = [] {
        using s     = L<>::append<short>;
        using si    = s::append<int>;
        using sil   = si::append<long>;
        using siluu = sil::append<unsigned, unsigned>;

        using s_expected     = L<short>;
        using si_expected    = L<short, int>;
        using sil_expected   = L<short, int, long>;
        using siluu_expected = L<short, int, long, unsigned, unsigned>;

        expect(constant<std::same_as<s, s_expected>>);
        expect(constant<std::same_as<si, si_expected>>);
        expect(constant<std::same_as<sil, sil_expected>>);
        expect(constant<std::same_as<siluu, siluu_expected>>);
    };

    tag("sstd") / "type_list::append<> is noop"_test = [] {
        using empty_before = L<>;
        using one_before   = L<short>;
        using two_before   = L<short, long>;

        using empty_after = empty_before::append<>;
        using one_after   = one_before::append<>;
        using two_after   = two_before::append<>;

        expect(constant<std::same_as<empty_before, empty_after>>);
        expect(constant<std::same_as<one_before, one_after>>);
        expect(constant<std::same_as<two_before, two_after>>);
    };

    // type_list<T...>::concat<type_list<U...>>:

    tag("sstd") / "type_list can be build with type_list_concat"_test = [] {
        using si       = sstd::type_list_concat<L<short>, L<int>>;
        using sisi     = sstd::type_list_concat<si, si>;
        using sisisisi = sstd::type_list_concat<sisi, sisi>;

        using si_expected       = L<short, int>;
        using sisi_expected     = L<short, int, short, int>;
        using sisisisi_expected = L<short, int, short, int, short, int, short, int>;

        expect(constant<std::same_as<si, si_expected>>);
        expect(constant<std::same_as<sisi, sisi_expected>>);
        expect(constant<std::same_as<sisisisi, sisisisi_expected>>);
    };

    tag("sstd") / "type_list_concat<...> for empty list is noop"_test = [] {
        using empty_before = L<>;
        using one_before   = L<short>;
        using two_before   = L<short, long>;

        using empty_after_A = sstd::type_list_concat<L<>, empty_before>;
        using one_after_A   = sstd::type_list_concat<L<>, one_before>;
        using two_after_A   = sstd::type_list_concat<L<>, two_before>;

        using empty_after_B = sstd::type_list_concat<empty_before, L<>>;
        using one_after_B   = sstd::type_list_concat<one_before, L<>>;
        using two_after_B   = sstd::type_list_concat<two_before, L<>>;

        expect(constant<std::same_as<empty_before, empty_after_A>>);
        expect(constant<std::same_as<one_before, one_after_A>>);
        expect(constant<std::same_as<two_before, two_after_A>>);

        expect(constant<std::same_as<empty_before, empty_after_B>>);
        expect(constant<std::same_as<one_before, one_after_B>>);
        expect(constant<std::same_as<two_before, two_after_B>>);
    };

    // template_invoke_with_list:

    tag("sstd")
        / "template_invocable_with_list_with_list is correct for sample set of standard traits"_test =
        [] {
            // Unary predicate standard traits
            expect(constant<not sstd::template_invocable_with_list<std::is_void, L<>>>);
            expect(constant<sstd::template_invocable_with_list<std::is_void, L<int>>>);
            expect(constant<not sstd::template_invocable_with_list<std::is_void, L<int, int>>>);
            expect(
                constant<not sstd::template_invocable_with_list<std::is_void, L<int, int, int>>>);

            // Binary predicate standard traits
            expect(constant<not sstd::template_invocable_with_list<std::is_swappable_with, L<>>>);
            expect(
                constant<not sstd::template_invocable_with_list<std::is_swappable_with, L<int>>>);
            expect(
                constant<sstd::template_invocable_with_list<std::is_swappable_with, L<int, int>>>);
            expect(constant<not sstd::template_invocable_with_list<std::is_swappable_with,
                                                                   L<int, int, int>>>);

            // N-ary predicate standard traits
            expect(not constant<sstd::template_invocable_with_list<std::is_constructible, L<>>>);
            expect(constant<sstd::template_invocable_with_list<std::is_constructible, L<int>>>);
            expect(
                constant<sstd::template_invocable_with_list<std::is_constructible, L<int, int>>>);
            expect(constant<
                   sstd::template_invocable_with_list<std::is_constructible, L<int, int, int>>>);

            // Unary non-predicate standard traits
            expect(constant<not sstd::template_invocable_with_list<std::rank, L<>>>);
            expect(constant<sstd::template_invocable_with_list<std::rank, L<int>>>);
            expect(constant<not sstd::template_invocable_with_list<std::rank, L<int, int>>>);
            expect(constant<not sstd::template_invocable_with_list<std::rank, L<int, int, int>>>);

            // Binary non-predicate standard traits
            expect(not constant<sstd::template_invocable_with_list<std::is_swappable_with, L<>>>);
            expect(
                not constant<sstd::template_invocable_with_list<std::is_swappable_with, L<int>>>);
            expect(
                constant<sstd::template_invocable_with_list<std::is_swappable_with, L<int, int>>>);
            expect(not constant<
                   sstd::template_invocable_with_list<std::is_swappable_with, L<int, int, int>>>);

            // N-ary non-predicate standard traits
            expect(constant<sstd::template_invocable_with_list<std::common_type, L<>>>);
            expect(constant<sstd::template_invocable_with_list<std::common_type, L<int>>>);
            expect(constant<sstd::template_invocable_with_list<std::common_type, L<int, int>>>);
            expect(
                constant<sstd::template_invocable_with_list<std::common_type, L<int, int, int>>>);

            // Miscellaneous predicate standard traits
            expect(constant<sstd::template_invocable_with_list<std::conjunction, L<>>>);
            expect(constant<sstd::template_invocable_with_list<std::conjunction, L<int>>>);
            expect(constant<sstd::template_invocable_with_list<std::conjunction, L<int, int>>>);
            expect(
                constant<sstd::template_invocable_with_list<std::conjunction, L<int, int, int>>>);
        };

    tag("sstd")
        / "template_invoke_with_list is correct for sample set of standard traits in a type_list"_test =
        [] {
            // Unary predicate standard traits
            expect(constant<std::same_as<sstd::template_invoke_with_list_t<std::is_void, L<int>>,
                                         std::is_void<int>>>);

            // Binary predicate standard traits
            expect(constant<std::same_as<
                       sstd::template_invoke_with_list_t<std::is_swappable_with, L<int, int>>,
                       std::is_swappable_with<int, int>>>);

            // N-ary predicate standard traits
            expect(constant<
                   std::same_as<sstd::template_invoke_with_list_t<std::is_constructible, L<int>>,
                                std::is_constructible<int>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_list_t<std::is_constructible, L<int, int>>,
                       std::is_constructible<int, int>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_list_t<std::is_constructible, L<int, int, int>>,
                       std::is_constructible<int, int, int>>>);

            // Unary non-predicate standard traits
            expect(constant<std::same_as<sstd::template_invoke_with_list_t<std::rank, L<int>>,
                                         std::rank<int>>>);

            // Binary non-predicate standard traits
            expect(constant<std::same_as<
                       sstd::template_invoke_with_list_t<std::is_swappable_with, L<int, int>>,
                       std::is_swappable_with<int, int>>>);

            // N-ary non-predicate standard traits
            expect(constant<std::same_as<sstd::template_invoke_with_list_t<std::common_type, L<>>,
                                         std::common_type<>>>);
            expect(
                constant<std::same_as<sstd::template_invoke_with_list_t<std::common_type, L<int>>,
                                      std::common_type<int>>>);
            expect(constant<
                   std::same_as<sstd::template_invoke_with_list_t<std::common_type, L<int, int>>,
                                std::common_type<int, int>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_list_t<std::common_type, L<int, int, int>>,
                       std::common_type<int, int, int>>>);

            // Miscellaneous predicate standard traits
            expect(constant<std::same_as<sstd::template_invoke_with_list_t<std::conjunction, L<>>,
                                         std::conjunction<>>>);
            expect(
                constant<std::same_as<sstd::template_invoke_with_list_t<std::conjunction, L<int>>,
                                      std::conjunction<int>>>);
            expect(constant<
                   std::same_as<sstd::template_invoke_with_list_t<std::conjunction, L<int, int>>,
                                std::conjunction<int, int>>>);
            expect(constant<std::same_as<
                       sstd::template_invoke_with_list_t<std::conjunction, L<int, int, int>>,
                       std::conjunction<int, int, int>>>);
        };
};
