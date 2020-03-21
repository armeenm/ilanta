#pragma once

#include "ilanta/hal/hw/i2c.hpp"

#include <cstdint>

namespace ilanta {

class PCA9685 {
public:
  [[nodiscard]] PCA9685(I2C*, std::uint16_t addr = DEFAULT_ADDR);

  PCA9685(PCA9685 const&) = delete;
  [[nodiscard]] PCA9685(PCA9685&&) noexcept = default;

  auto operator=(PCA9685 const&) -> PCA9685& = delete;
  auto operator=(PCA9685&&) noexcept -> PCA9685& = default;

  ~PCA9685() = default;

  [[nodiscard]] auto freq() const noexcept -> std::uint16_t;
  auto freq(std::uint16_t) noexcept -> bool;

  [[nodiscard]] auto duty_cycle(std::uint8_t channel) -> std::uint8_t;
  auto duty_cycle(std::uint8_t channel, std::uint8_t duty_cycle) -> bool;

  [[nodiscard]] auto addr() const noexcept -> std::uint16_t;
  auto addr(std::uint16_t) noexcept -> void;

private:
  auto static constexpr DEFAULT_ADDR = std::uint16_t{0x40};

  I2C* bus_;
  std::uint16_t addr_;
}

} // namespace ilanta
