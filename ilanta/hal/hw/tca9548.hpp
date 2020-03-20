#pragma once

#include "ilanta/hal/hw/i2c.hpp"

namespace ilanta {

class TCA9548 {
public:
  [[nodiscard]] TCA9548(I2C*, std::uint16_t addr = DEFAULT_ADDR);

  TCA9548(TCA9548 const&) = delete;
  [[nodiscard]] TCA9548(TCA9548&&) noexcept = default;

  auto operator=(TCA9548 const&) -> TCA9548& = delete;
  auto operator=(TCA9548&&) noexcept -> TCA9548& = default;

  ~TCA9548() = default;

  auto val(std::uint8_t port) const -> bool;

  [[nodiscard]] auto addr() const noexcept -> std::uint16_t;
  auto addr(std::uint16_t) noexcept -> void;

private:
  auto static constexpr DEFAULT_ADDR = std::uint16_t{0x70};

  I2C* bus_;
  std::uint16_t addr_;
};

} // namespace ilanta
