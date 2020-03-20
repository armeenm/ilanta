#pragma once

#include <concepts>

namespace ilanta {

// clang-format off
template <typename T> concept Addressable = requires(T x) {
  { x.addr() } -> std::unsigned_integral;
};
// clang-format on

} // namespace ilanta
