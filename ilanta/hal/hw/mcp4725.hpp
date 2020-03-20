#pragma once

#include "ilanta/hal/hw/i2c.hpp"
#include <cstdint>
#include <optional>

namespace ilanta {

class MCP4725 {
public:
  [[nodiscard]] MCP4725(I2C*, std::uint16_t addr = DEFAULT_ADDR);

  MCP4725(MCP4725 const&) = delete;
  [[nodiscard]] MCP4725(MCP4725&&) noexcept = default;

  auto operator=(MCP4725 const&) -> MCP4725& = delete;
  auto operator=(MCP4725&&) noexcept -> MCP4725& = default;

  ~MCP4725() = default;

  [[nodiscard]] auto val() const -> std::optional<std::uint16_t>;
  auto val(std::uint16_t) const -> bool;

  [[nodiscard]] auto addr() const noexcept -> std::uint16_t;
  auto addr(std::uint16_t) noexcept -> void;

private:
  auto static constexpr DEFAULT_ADDR = std::uint16_t{0x62};

  I2C* bus_;
  std::uint16_t addr_;
};

} // namespace ilanta
