#pragma once

#include "ilanta/hal/hw/i2cdevice.hpp"
#include "ilanta/util/result.hpp"

#include <cstdint>
#include <system_error>
#include <utility>

namespace ilanta {

class PCA9685 : public I2CDevice {
public:
  auto constexpr inline static default_addr = 0x40;

  PCA9685(PCA9685 const&) = delete;
  [[nodiscard]] PCA9685(PCA9685&&) noexcept = default;

  auto operator=(PCA9685 const&) -> PCA9685& = delete;
  auto operator=(PCA9685&&) noexcept -> PCA9685& = default;

  ~PCA9685() = default;

  auto reset() const noexcept -> void;

  auto addr(std::uint16_t) const noexcept -> std::error_code;
  auto freq(std::uint16_t) const noexcept -> std::error_code;

  [[nodiscard]] auto duty_cycle(std::uint8_t channel) -> Result<std::uint16_t, std::error_code>;
  auto duty_cycle(std::uint8_t channel, std::uint16_t duty_cycle) -> std::error_code;
};

} // namespace ilanta
