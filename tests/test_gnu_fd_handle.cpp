#include <boost/ut.hpp> // import boost.ut;

/// @file What is tested here?
///
/// As fd_handle is not uspposed to be used on its own,
/// it is not directly tested any more that is here.
/// However other functionality is tested indirectly by
/// tests of functionality which are implemented using fd_handle.

#include "gnu_utils/fd_handle.hpp"

#include <concepts>

int main() {
    using namespace boost::ut;
    cfg<override> = { .tag = { "gnu" } };

    tag("gnu") / "standalone fd_handle can not be constructed"_test = [] {
        expect(not std::default_initializable<ger::gnu::fd_handle>);
    };

    tag("gnu") / "fd_handle is movable but not copyable"_test = [] {
        expect(std::movable<ger::gnu::fd_handle>);
        expect(not std::copyable<ger::gnu::fd_handle>);
    };
}
