#pragma once

#include "ilanta/hal/hw/smbus.hpp"
#include "ilanta/util/result.hpp"

#include <cstdint>
#include <system_error>
#include <utility>

namespace ilanta {

class MCP4725 {
public:
  [[nodiscard]] MCP4725(SMBus bus) : MCP4725{std::move(bus), 0x62} {}
  [[nodiscard]] MCP4725(SMBus, std::uint16_t addr);

  MCP4725(MCP4725 const&) = delete;
  [[nodiscard]] MCP4725(MCP4725&&) noexcept = default;

  auto operator=(MCP4725 const&) -> MCP4725& = delete;
  auto operator=(MCP4725&&) noexcept -> MCP4725& = default;

  ~MCP4725() = default;

  auto addr(std::uint16_t) const noexcept -> std::error_code;

  [[nodiscard]] auto val() const noexcept -> Result<std::uint16_t, std::error_code>;
  auto val(std::uint16_t) const noexcept -> std::error_code;

private:
  SMBus bus_;
};

} // namespace ilanta
