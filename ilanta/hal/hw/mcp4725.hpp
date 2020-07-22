#pragma once

#include "ilanta/util/result.hpp"
#include "ilanta/hal/hw/i2cdevice.hpp"

#include <cstdint>
#include <system_error>
#include <filesystem>
#include <span>
#include <utility>

namespace ilanta {

class MCP4725 : public I2CDevice {
public:
  [[nodiscard]] MCP4725(std::filesystem::path const& path) : MCP4725{std::move(path), 0x62} {}
  [[nodiscard]] MCP4725(std::filesystem::path const& path, std::uint16_t addr);

  MCP4725(MCP4725 const&) = default;
  [[nodiscard]] MCP4725(MCP4725&&) noexcept = default;

  auto operator=(MCP4725 const&) -> MCP4725& = default;
  auto operator=(MCP4725&&) noexcept -> MCP4725& = default;

  ~MCP4725() = default;

  [[nodiscard]] auto val() const noexcept -> Result<std::uint16_t, std::error_code>;
  auto val(std::uint16_t) const noexcept -> std::error_code;

private:
  auto transfer(std::span<std::uint8_t const> arr) const noexcept -> std::error_code;
};

} // namespace ilanta
