#include <boost/ut.hpp> // import boost.ut;

#include <concepts>

#include "type_utils.hpp"

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

};
