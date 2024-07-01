#include <config.h>

#include <boost/ut.hpp> // import boost.ut;

#include <cerrno>
#include <system_error>

#include "sstd.hpp"
int main() {
    using namespace boost::ut;
    using namespace ger;
    cfg<override> = { .tag = { "sstd" } };

    tag("sstd") / "thrown generic system error matches errno"_test = [] {
        expect(throws<std::system_error>([] {
            errno = EBADF;
            sstd::throw_generic_system_error();
            expect(not "supposed to happen"_b);
        }));

        try {
            errno = EIO;
            sstd::throw_generic_system_error();
            expect(not "supposed to happen"_b);
        } catch (const std::system_error& err) {
            expect(err.code() == std::make_error_code(std::errc::io_error));
        } catch (...) { expect(not "system_error"_b); };
    };

    tag("sstd") / "thrown partial system io error matches errno"_test = [] {
        expect(throws<std::system_error>([] {
            errno = EBADF;
            sstd::throw_partial_system_io_error(1, 10);
            expect(not "supposed to happen"_b);
        }));

        try {
            errno = EIO;
            sstd::throw_partial_system_io_error(1, 10);
            expect(not "supposed to happen"_b);
        } catch (const std::system_error& err) {
            expect(err.code() == std::make_error_code(std::errc::io_error));
        } catch (...) { expect(not "system_error"_b); };
    };

    tag("sstd") / "throwing generic system error works even with errno = 0"_test = [] {
        expect(throws<std::system_error>([] {
            errno = 0;
            sstd::throw_generic_system_error();
            expect(not "supposed to happen"_b);
        }));

        try {
            errno = 0;
            sstd::throw_generic_system_error();
            expect(not "supposed to happen"_b);
        } catch (const std::system_error& err) {
            // std::error_code has explicit operator bool to check if it has valid error.
            const auto has_valid_error = bool{ err.code() };
            expect(not has_valid_error);
        } catch (...) { expect(not "system_error"_b); };
    };

    tag("sstd") / "throwing partial system io error works even with errno = 0"_test = [] {
        expect(throws<std::system_error>([] {
            errno = 0;
            sstd::throw_partial_system_io_error(1, 10);
            expect(not "supposed to happen"_b);
        }));

        try {
            errno = 0;
            sstd::throw_partial_system_io_error(1, 10);
            expect(not "supposed to happen"_b);
        } catch (const std::system_error& err) {
            // std::error_code has explicit operator bool to check if it has valid error.
            const auto has_valid_error = bool{ err.code() };
            expect(not has_valid_error);
        } catch (...) { expect(not "system_error"_b); };
    };
}
