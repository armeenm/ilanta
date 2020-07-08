#include "ilanta/hal/hw/tca9548.hpp"

#include <cassert>

namespace ilanta {

TCA9548::TCA9548(SMBus&& bus, std::uint16_t const addr_in) : bus_{std::move(bus)} { addr(addr_in); }

auto TCA9548::val(std::uint8_t const port) const -> std::error_code {
  assert(port < 8U);

  // TODO: Implement

  return {};
}

auto TCA9548::addr(std::uint16_t const addr) const noexcept -> std::error_code {
  return bus_.addr(addr);
}

} // namespace ilanta
