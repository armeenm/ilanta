#pragma once

#include <cmath>
#include <compare>
#include <ostream>

#include "ilanta/math/util.hpp"
#include "ilanta/util/format.hpp"
#include "ilanta/util/generics.hpp"

namespace ilanta {

/**
 * Translational pose structure
 */
template <Arithmetic T = float>
class PoseTL {
 public:
  [[nodiscard]] constexpr explicit PoseTL() noexcept {}
  [[nodiscard]] constexpr PoseTL(T x, T y, T z) noexcept : x_{x}, y_{y}, z_{z} {}

  constexpr PoseTL(PoseTL<T> const&) = default;
  constexpr PoseTL(PoseTL<T>&&) = default;

  auto constexpr operator=(PoseTL<T> const&) -> PoseTL<T>& = default;
  auto constexpr operator=(PoseTL<T> &&) -> PoseTL<T>& = default;

  ~PoseTL() = default;

  [[nodiscard]] auto constexpr operator+(PoseTL<T> const& other) const noexcept -> PoseTL {
    return PoseTL{x_ + other.x(), y_ + other.y(), z_ + other.z()};
  }

  [[nodiscard]] auto constexpr operator-(PoseTL<T> const& other) const noexcept -> PoseTL {
    return PoseTL{x_ - other.x(), y_ - other.y(), z_ - other.z()};
  }

  [[nodiscard]] auto constexpr operator==(PoseTL<T> const&) const noexcept -> bool = default;

  [[nodiscard]] auto constexpr operator<=>(PoseTL<T> const& other) const noexcept
      -> std::partial_ordering {
    return l2sqnorm() <=> other.l2sqnorm();
  };

  [[nodiscard]] auto constexpr x() const noexcept -> T { return x_; }
  [[nodiscard]] auto constexpr y() const noexcept -> T { return y_; }
  [[nodiscard]] auto constexpr z() const noexcept -> T { return z_; }

  auto constexpr x(T x) noexcept -> void { x_ = x; }
  auto constexpr y(T y) noexcept -> void { y_ = y; }
  auto constexpr z(T z) noexcept -> void { z_ = z; }

  [[nodiscard]] auto constexpr l2sqnorm() const noexcept -> T {
    return x_ * x_ + y_ * y_ + z_ * z_;
  }

  [[nodiscard]] auto constexpr l2norm() const noexcept -> T { return sqrt(l2sqnorm()); }

 private:
  T x_ = 0.0;
  T y_ = 0.0;
  T z_ = 0.0;
};

template <typename T>
inline auto operator<<(std::ostream& os, PoseTL<T> const& pose) -> std::ostream& {
  os << format("PoseTL(x = {}, y = {}, z = {})", pose.x(), pose.y(), pose.z());
  return os;
}

}  // namespace ilanta
