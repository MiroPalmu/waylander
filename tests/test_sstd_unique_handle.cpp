// Copyright (C) 2024 Miro Palmu.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file.  If not, see <https://www.gnu.org/licenses/>.


#include <boost/ut.hpp> // import boost.ut;

#include <concepts>
#include <utility>

#include "guilander/sstd.hpp"

struct mock_resource {
    bool is_active{ true };
    void free() { is_active = false; }

    mock_resource()                = default;
    mock_resource(mock_resource&&) = default;
    /// Tests that active resource is never overwritten.
    mock_resource& operator=(mock_resource&& rhs) {
        boost::ut::expect(not is_active);
        is_active = rhs.is_active;
        return *this;
    }
};

void checked_free_er(mock_resource& x) {
    boost::ut::expect(x.is_active);
    x.free();
}

int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "sstd" } };

    tag("sstd") / "unique_handle is not usable without resource"_test = [] {
        using handler = sstd::unique_handle<mock_resource, checked_free_er>;
        expect(not std::default_initializable<handler>);
        expect(std::constructible_from<handler, mock_resource&&>);
    };

    tag("sstd") / "unique_handle is movable but not copyable"_test = [] {
        using handler = sstd::unique_handle<mock_resource, checked_free_er>;
        expect(std::movable<handler>);
        expect(not std::copyable<handler>);
    };

    tag("sstd") / "unique_handle dtor releases the handled resource"_test = [] {
        // Releaser calls boost::ut::expect to check that resource is active at time of release.
        using handler = sstd::unique_handle<mock_resource, checked_free_er>;

        auto handle = handler{ mock_resource{} };
        expect(handle.is_active);
        handle.~unique_handle();
        expect(not handle.is_active);

        // expect call in checked_free_er should fail if it called on non-active resource.
    };

    tag("sstd") / "unique_handle dtor does not call Releaser when it is moved from"_test = [] {
        // Releaser calls boost::ut::expect to check that resource is active at time of release.
        using handler = sstd::unique_handle<mock_resource, checked_free_er>;
        auto handleA  = handler{ mock_resource{} };
        auto handleB{ std::move(handleA) };

        // expect call in checked_free_er should fail if it called on non-active resource.
    };

    tag("sstd") / "unique_handle move ctor uses move ctor of the resource"_test = [] {
        // Releaser calls boost::ut::expect to check that resource is active at time of release.
        using handler = sstd::unique_handle<mock_resource, checked_free_er>;

        auto handleA = handler{ mock_resource{} };
        expect(handleA.is_active);
        handleA.~unique_handle();
        expect(not handleA.is_active);

        auto handleB{ std::move(handleA) };
        // If move ctor of resource in handleB is used, it should be inactive.
        expect(not handleB.is_active);

        // expect call in checked_free_er should fail if it called on non-active resource.
    };

    tag("sstd") / "move assignment calls Release on resource before overwriting it"_test = [] {
        // Releaser calls boost::ut::expect to check that resource is active at time of release.
        using handler = sstd::unique_handle<mock_resource, checked_free_er>;

        auto handleA = handler{ mock_resource{} };
        auto handleB = handler{ mock_resource{} };
        // mock_resource's move assignment checks that it is not active,
        // i.e. move assignment of unique_handle will release its resources before
        // calling the move assignment of moved resource.
        handleA = std::move(handleB);
    };

#ifdef __GNUG__
    tag("sstd") / "self move assignment does not double release"_test = [] {
        // Releaser calls boost::ut::expect to check that resource is active at time of release.
        using handler = sstd::unique_handle<mock_resource, checked_free_er>;

        auto handle = handler{ mock_resource{} };
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wself-move"
        handle = std::move(handle);
#    pragma GCC diagnostic pop
    };
#endif
}
