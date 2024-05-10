#include <boost/ut.hpp> // import boost.ut;

#include <tuple>
#include <type_traits>

#include "type_utils.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "sstd" } };

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
