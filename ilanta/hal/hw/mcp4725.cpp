#include "ilanta/hal/hw/mcp4725.hpp"
#include "ilanta/hal/hw/i2c.hpp"
#include "ilanta/util/errors.hpp"

#include <array>
#include <spdlog/spdlog.h>
#include <vector>

namespace ilanta {

MCP4725::MCP4725(I2C* const bus, std::uint16_t const addr) : bus_(bus), addr_(addr) {
  spdlog::info("Constructing MCP4725");

  [[unlikely]] if (!bus) err_log_throw("Invalid I2C bus");
}

auto MCP4725::val() const -> std::optional<std::uint16_t> {
  auto data = std::array<std::uint8_t, 2>{};
  auto msg = std::array{
      I2C::Message{.addr = addr_, .flags = I2C::READ, .len = data.size(), .buf = data.data()}};

  if (bus_->transfer(msg))
    return std::nullopt;

  // Reconstruct 12-bit value
  return ((data[0] << 4) | (data[1] >> 4)) & 0xFF;
}

auto MCP4725::val(std::uint16_t const val) const -> std::error_code {
  assert(val >= 0 && val <= 4095);

  auto data =
      std::array{static_cast<std::uint8_t>(val >> 8), static_cast<std::uint8_t>(val & 0xFF)};

  auto msg = I2C::Message{.addr = addr_, .flags = 0, .len = data.size(), .buf = data.data()};

  return bus_->transfer({&msg, 1});
}

auto MCP4725::addr() const noexcept -> std::uint16_t { return addr_; }
auto MCP4725::addr(std::uint16_t addr) noexcept -> void { addr_ = addr; }

} // namespace ilanta
