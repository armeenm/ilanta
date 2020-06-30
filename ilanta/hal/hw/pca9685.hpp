#pragma once

#include "ilanta/hal/hw/i2c.hpp"

#include <cstdint>

namespace ilanta {

class PCA9685 {
public:
  [[nodiscard]] PCA9685(I2C&, std::uint16_t addr = 0x40);

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
  enum class Reg : std::uint8_t {
    MODE1,
    MODE2,
    SUBADR1,
    SUBADR2,
    SUBADR3,
    ALLCALLADR,
    LED0_ON_L,
    LED0_ON_H,
    LED0_OFF_L,
    LED0_OFF_H,
    ALL_LED_ON_L = 0xFA,
    ALL_LED_ON_H,
    ALL_LED_OFF_L,
    ALL_LED_OFF_H,
    PRE_SCALE,
    TESTMODE
  };

  enum class Mode1Reg : std::uint8_t {
    ALLCALL = 0x01,
    SUB3 = 0x02,
    SUB2 = 0x04,
    SUB1 = 0x08,
    SLEEP = 0x10,
    AI = 0x20,
    EXTCLK = 0x40,
    RESTART = 0x80
  };

  enum class Mode2Reg : std::uint8_t {
    OUTNE_0 = 0x01,
    OUTNE_1 = 0x02,
    OUTDRV = 0x04,
    OCH = 0x08,
    INVRT = 0x10
  };

  I2C* bus_;
  std::uint16_t addr_;
};

} // namespace ilanta
