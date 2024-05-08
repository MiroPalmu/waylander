#pragma once

#include "construct_allocator_adapter.hpp"

namespace ger {
namespace sstd {
using byte_vec_alloc =
    sstd::allocator::construct_allocator_adapter<sstd::allocator::default_init_construct,
                                                 std::byte>;
using byte_vec = std::vector<std::byte, byte_vec_alloc>;
} // namespace sstd
} // namespace ger
