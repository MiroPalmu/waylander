#pragma once

/// @file Contains primitives used in protocol.hpp implementation.

#include <cstdint>
#include <functional>

#include "bit_fiddling.hpp"
#include "linux_utils/file_descriptor.hpp"
#include "sstd.hpp"

namespace ger {
namespace wl {

struct alignas(4) dynamic_argument {
    std::uint32_t size;
};

struct alignas(4) Wint : sstd::integral_like<std::int32_t> {};
struct alignas(4) Wuint : sstd::integral_like<std::uint32_t> {};
struct alignas(4) Wfixed {
    bool is_negative : 1;
    unsigned mantissa : 23;
    unsigned exponent : 8;
};
struct alignas(4) Wstring : dynamic_argument {};
struct alignas(4) Wobject : sstd::integral_like<Wuint::integral_type> {};
struct alignas(4) Wnew_id : sstd::integral_like<Wobject::integral_type> {};
struct alignas(4) Warray : dynamic_argument {};
using Wfd = std::reference_wrapper<linux::fd_type>;

struct alignas(2) Wmessage_size_t : sstd::integral_like<std::uint16_t> {};
struct alignas(2) Wopcode : sstd::integral_like<std::uint16_t> {};

struct alignas(4) message_header {
    Wobject object_id;
    Wmessage_size_t size;
    Wopcode opcode;
};

} // namespace wl
} // namespace ger
