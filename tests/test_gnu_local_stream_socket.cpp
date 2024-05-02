#include <boost/ut.hpp> // import boost.ut;

#include "gnu_utils/local_stream_socket.hpp"
int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "gnu" } };

    tag("gnu") / "local_stream_socket pair can be constructed"_test = [] {
        [[maybe_unused]] auto [_, _] = ger::gnu::open_local_stream_socket_pair();
        expect(true);
    };
}
