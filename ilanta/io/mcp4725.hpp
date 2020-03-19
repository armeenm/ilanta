#pragma once

#include "ilanta/io/i2c.hpp"
#include <cstdint>
#include <optional>

namespace ilanta {

class MCP4725 {
public:
  [[nodiscard]] MCP4725(I2C*, std::uint16_t addr = DEFAULT_ADDR);

  [[nodiscard]] auto val() const -> std::optional<std::uint16_t>;
  auto val(std::uint16_t) const -> bool;

private:
  std::uint16_t static constexpr DEFAULT_ADDR = 0x62;

  I2C* bus_;
  std::uint16_t addr_;
};

} // namespace ilanta
