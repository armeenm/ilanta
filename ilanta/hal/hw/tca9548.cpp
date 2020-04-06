#include "ilanta/hal/hw/tca9548.hpp"
#include "ilanta/util/errors.hpp"

#include <spdlog/spdlog.h>

namespace ilanta {

TCA9548::TCA9548(I2C& bus, std::uint16_t const addr) : bus_(bus), addr_(addr) {
  spdlog::info("Constructing TCA9548");
}

auto TCA9548::val(std::uint8_t const port) const -> std::error_code {
  assert(port < 8);

  auto data = std::array{static_cast<std::uint8_t>(1 << port)};
  auto msg =
      std::array{I2C::Message{.addr = addr_, .flags = 0, .len = data.size(), .buf = data.data()}};

  return bus_.transfer(msg);
}

auto TCA9548::addr() const noexcept -> std::uint16_t { return addr_; }
auto TCA9548::addr(std::uint16_t const addr) noexcept -> void { addr_ = addr; }

} // namespace ilanta
