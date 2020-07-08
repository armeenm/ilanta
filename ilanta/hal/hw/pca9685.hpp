#pragma once

#include "ilanta/hal/hw/smbus.hpp"
#include "ilanta/util/result.hpp"

#include <cstdint>
#include <system_error>

namespace ilanta {

class PCA9685 {
public:
  [[nodiscard]] PCA9685(SMBus&& bus) : PCA9685{std::move(bus), 0x40} {}
  [[nodiscard]] PCA9685(SMBus&&, std::uint16_t addr);

  PCA9685(PCA9685 const&) = delete;
  [[nodiscard]] PCA9685(PCA9685&&) noexcept = default;

  auto operator=(PCA9685 const&) -> PCA9685& = delete;
  auto operator=(PCA9685&&) noexcept -> PCA9685& = default;

  ~PCA9685() = default;

  auto reset() const noexcept -> void;

  [[nodiscard]] auto freq() const noexcept -> std::uint16_t;
  auto freq(std::uint16_t) noexcept -> std::error_code;

  [[nodiscard]] auto duty_cycle(std::uint8_t channel) -> Result<std::uint16_t, std::error_code>;
  auto duty_cycle(std::uint8_t channel, std::uint16_t duty_cycle) -> std::error_code;

  auto addr(std::uint16_t) const noexcept -> std::error_code;

private:
  SMBus bus_;
  std::uint16_t freq_;
};

} // namespace ilanta
