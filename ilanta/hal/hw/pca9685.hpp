#pragma once

#include "ilanta/util/result.hpp"
#include "ilanta/hal/hw/i2cdevice.hpp"

#include <cstdint>
#include <system_error>
#include <utility>
#include <filesystem>

namespace ilanta {

class PCA9685 : public I2CDevice {
public:
  PCA9685(std::filesystem::path const& path) : PCA9685{std::move(path), 0x40} {}
  PCA9685(std::filesystem::path const& path, std::uint16_t addr);

  PCA9685(PCA9685 const&) = default;
  PCA9685(PCA9685&&) noexcept = default;

  auto operator=(PCA9685 const&) -> PCA9685& = default;
  auto operator=(PCA9685&&) noexcept -> PCA9685& = default;

  ~PCA9685() = default;

  auto reset() const noexcept -> std::error_code;

  auto freq(std::uint16_t) const noexcept -> std::error_code;

  [[nodiscard]] auto duty_cycle(std::uint8_t channel) -> Result<std::uint16_t, std::error_code>;
  auto duty_cycle(std::uint8_t channel, std::uint16_t duty_cycle) -> std::error_code;
};

} // namespace ilanta
