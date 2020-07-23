#pragma once

#include <cmath>
#include <concepts>
#include <type_traits>

#include "ilanta/util/generics.hpp"

namespace ilanta {

template <Arithmetic T, typename Res = std::conditional_t<std::is_integral_v<T>, double, T>>
auto inline constexpr sqrt(T const x, unsigned long iterations = 1000) -> Res {
  if constexpr (!std::is_constant_evaluated())
    return std::sqrt(x);
  else {
    auto guess = Res(x / Res{2.0});

    for (; iterations > 0; --iterations) guess = Res{0.5} * (guess + x / guess);

    return guess;
  }
}

template <std::floating_point T>
auto inline constexpr approx_equal(T x, T y, T tolerance) -> bool {
  return std::abs(x - y) <= tolerance;
}

}  // namespace ilanta
