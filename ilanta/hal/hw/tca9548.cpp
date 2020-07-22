#include "ilanta/hal/hw/tca9548.hpp"
#include "ilanta/util/errors.hpp"

#include <cassert>

namespace ilanta {

TCA9548::TCA9548(std::filesystem::path const& path, std::uint16_t const addr_in)
  : I2CDevice{std::move(path)} {

  auto const e = addr(addr_in);
  if (e)
    err_log_throw("Failed to set TCA9548 address to {}: {} [{}]", addr_in, e.message(), e.value());
}

auto TCA9548::port(std::uint8_t const port) const noexcept -> std::error_code {
  assert(port < 8U);

  // TODO: Implement

  return {};
}

} // namespace ilanta
