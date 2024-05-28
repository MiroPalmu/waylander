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
        using list   = sstd::type_list<int, double, float>;
        using generic_lambda = decltype([](auto) -> short { return {};});
        using mapped = list::map<unsigned(int), generic_lambda>;
        using expected = sstd::type_list<unsigned, short, short>;

        expect(constant<std::same_as<expected, mapped>>);
    };

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
