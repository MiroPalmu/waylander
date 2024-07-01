#include <config.h>

#include <boost/ut.hpp> // import boost.ut;

#include <string_view>
#include <tuple>
#include <type_traits>

#include "type_utils.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "sstd" } };

    tag("sstd") / "to_tuple returns copy of a give object as a tuple"_test = [] {
        struct Foo {
            unsigned a;
            int b;
            std::string_view c;
        };

        const auto foo       = Foo{ 42, 43, "foo object" };
        const auto foo_tuple = sstd::to_tuple(foo);

        using tuple_type          = std::remove_cvref_t<decltype(foo_tuple)>;
        using expected_tuple_type = std::tuple<unsigned, int, std::string_view>;

        expect(std::is_same_v<tuple_type, expected_tuple_type>);

        expect(std::get<0>(foo_tuple) == foo.a);
        expect(std::get<1>(foo_tuple) == foo.b);
        expect(std::get<2>(foo_tuple) == foo.c);
    };

    tag("sstd") / "to_ref_tuple is tuple of const references"_test = [] {
        struct Foo {
            unsigned a;
            int b;
            std::string_view c;
        };

        const auto foo       = Foo{ 42, 43, "foo object" };
        const auto ref_tuple = sstd::to_ref_tuple(foo);

        using ref_tuple_type = std::remove_cvref_t<decltype(ref_tuple)>;
        using expected_ref_tuple_type =
            std::tuple<const unsigned&, const int&, const std::string_view&>;

        expect(std::is_same_v<ref_tuple_type, expected_ref_tuple_type>);

        expect(std::get<0>(ref_tuple) == foo.a);
        expect(std::get<1>(ref_tuple) == foo.b);
        expect(std::get<2>(ref_tuple) == foo.c);

        expect(std::addressof(std::get<0>(ref_tuple)) == std::addressof(foo.a));
        expect(std::addressof(std::get<1>(ref_tuple)) == std::addressof(foo.b));
        expect(std::addressof(std::get<2>(ref_tuple)) == std::addressof(foo.c));
    };
}
