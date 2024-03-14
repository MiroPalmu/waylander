#include <boost/ut.hpp> // import boost.ut;

#include "wayland/protocol_primitives.hpp"

int main() {
    using namespace boost::ut;
    using namespace ger::wl;

    static const auto wl_tag = tag("wayland");
    // Run wl_tag:
    cfg<override> = { .tag = { "wayland" } };

    wl_tag / "every primitive can be constructed from integral literals"_test = [] {
        constexpr auto i  = Wint{ 1 };
        constexpr auto ui = Wuint{ 2 };
        constexpr auto f  = Wfixed{ .is_negative = true, .mantissa = 3, .exponent = 4 };
        constexpr auto s  = Wstring{ 5 };
        constexpr auto o  = Wobject{ 6 };
        constexpr auto ni = Wnew_id{ 7 };
        constexpr auto a  = Warray{ 8 };
        constexpr auto ms = Wmessage_size_t{ 9 };
        constexpr auto op = Wopcode{ 10 };
        constexpr auto h  = message_header{ 11, 12, 13 };
        expect(i == 1);
        expect(ui == 2u);
        expect(f.is_negative == true);
        expect(s.size == 5u);
        expect(o == 6u);
        expect(ni == 7u);
        expect(a.size == 8u);
        expect(ms == 9u);
        expect(op == 10u);
        expect(h.object_id == 11u);
        expect(h.size == 12u);
        expect(h.opcode == 13u);
    };
}
