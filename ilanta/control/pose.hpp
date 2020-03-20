#pragma once

#include <cmath>

namespace ilanta {

/**
 * Translational pose structure
 */
class PoseTL {
public:
  [[nodiscard]] constexpr PoseTL(float x, float y, float z) : x_(x), y_(y), z_(z) {}

  constexpr PoseTL(PoseTL const&) = default;
  constexpr PoseTL(PoseTL&&) = default;

  auto constexpr operator=(PoseTL const&) -> PoseTL& = default;
  auto constexpr operator=(PoseTL &&) -> PoseTL& = default;

  ~PoseTL() = default;

  [[nodiscard]] auto constexpr operator+(PoseTL const& other) const noexcept -> PoseTL {
    return PoseTL{x_ + other.x(), y_ + other.y(), z_ + other.z()};
  }

  [[nodiscard]] auto constexpr operator-(PoseTL const& other) const noexcept -> PoseTL {
    return PoseTL{x_ - other.x(), y_ - other.y(), z_ - other.z()};
  }

  [[nodiscard]] auto constexpr operator==(PoseTL const&) const noexcept -> bool = default;

  [[nodiscard]] auto constexpr x() const noexcept -> float { return x_; }
  [[nodiscard]] auto constexpr y() const noexcept -> float { return y_; }
  [[nodiscard]] auto constexpr z() const noexcept -> float { return z_; }

  auto constexpr x(float x) noexcept -> void { x_ = x; }
  auto constexpr y(float y) noexcept -> void { y_ = y; }
  auto constexpr z(float z) noexcept -> void { z_ = z; }

  // TODO: Make it constexpr...
  auto l2norm() const -> float { return std::hypot(x_, y_, z_); }

private:
  float x_ = 0.0;
  float y_ = 0.0;
  float z_ = 0.0;
};

} // namespace ilanta
