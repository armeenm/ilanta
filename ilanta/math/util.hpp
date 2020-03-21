#pragma once

#include <cmath>
#include <concepts>

template <std::floating_point T> auto inline constexpr approx_equal(T x, T y, T tolerance) -> bool {
  return std::abs(x - y) <= tolerance;
}
