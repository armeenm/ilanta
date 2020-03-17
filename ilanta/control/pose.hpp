#pragma once

namespace ilanta {

/**
 * Translational pose structure
 */
struct PoseTL {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  auto operator+(PoseTL const& other) const noexcept -> PoseTL {
    return PoseTL{x + other.x, y + other.y, z + other.y};
  }
  auto operator-(PoseTL const& other) const noexcept -> PoseTL {
    return PoseTL{x - other.x, y - other.y, z - other.z};
  }
};

} // namespace ilanta
