#include "ilanta/hal/hw/mcp4725.hpp"
#include "ilanta/util/errors.hpp"

#include <array>
#include <cerrno>
#include <unistd.h>
#include <vector>

namespace ilanta {

template <std::size_t Size>
auto transfer(SMBus const& bus, std::array<std::uint8_t, Size> const& arr) noexcept
    -> std::error_code {

  auto const err = write(bus.native_handle(), arr.data(), arr.size());

  if (err != ssize_t(arr.size()))
    return errno_to_err_code(errno);

  return {};
}

MCP4725::MCP4725(SMBus bus, std::uint16_t const addr_in) : bus_{std::move(bus)} {
  if (addr(addr_in))
    err_log_throw("Failed to set MCP4725 address to {}", addr_in);
}

auto MCP4725::addr(std::uint16_t const addr_in) const noexcept -> std::error_code {
  return bus_.addr(addr_in);
}

auto MCP4725::val() const noexcept -> Result<std::uint16_t, std::error_code> {

  return std::error_code{};
}

auto MCP4725::val(std::uint16_t const val) const noexcept -> std::error_code {
  assert(val >= 0 && val < 4096);

  auto const arr =
      std::to_array<std::uint8_t>({0b01000000U, std::uint8_t(val >> 4), std::uint8_t(val << 4)});

  return transfer(bus_, arr);
}

} // namespace ilanta
