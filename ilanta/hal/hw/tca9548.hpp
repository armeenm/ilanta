#pragma once

#include "ilanta/hal/hw/i2cdevice.hpp"

#include <cstdint>
#include <filesystem>

namespace ilanta {

class TCA9548 : public I2CDevice {
public:
  [[nodiscard]] TCA9548(std::filesystem::path const& path) : TCA9548{std::move(path), 0x70} {}
  [[nodiscard]] TCA9548(std::filesystem::path const& path, std::uint16_t addr);

  TCA9548(TCA9548 const&) = default;
  TCA9548(TCA9548&&) noexcept = default;

  auto operator=(TCA9548 const&) -> TCA9548& = default;
  auto operator=(TCA9548&&) noexcept -> TCA9548& = default;

  ~TCA9548() = default;

  auto port(std::uint8_t) const noexcept -> std::error_code;
};

} // namespace ilanta
