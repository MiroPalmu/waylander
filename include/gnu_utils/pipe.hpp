#pragma once

#include <tuple>

namespace ger {
namespace gnu {

// tmp
struct in_pipe {
    int _;
};
struct out_pipe {
    int _;
};
struct inout_pipe {
    int _;
};

[[nodiscard]] auto open_pipe() -> std::tuple<in_pipe, out_pipe>;

} // namespace gnu
} // namespace ger
