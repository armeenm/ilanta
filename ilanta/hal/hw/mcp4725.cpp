#include "ilanta/hal/hw/mcp4725.hpp"
#include "ilanta/util/errors.hpp"

#include <array>
#include <cerrno>
#include <utility>
#include <unistd.h>

namespace ilanta {

MCP4725::MCP4725(std::filesystem::path const& path, std::uint16_t const addr_in) : I2CDevice{std::move(path)} {
  auto const e = addr(addr_in);
  if (e)
    err_log_throw("Failed to set MCP4725 address to {}: {} [{}]", addr_in, e.message(), e.value());
}

auto MCP4725::val() const noexcept -> Result<std::uint16_t, std::error_code> {
  // TODO
  return std::error_code{};
}

auto MCP4725::val(std::uint16_t const val) const noexcept -> std::error_code {
  assert(val >= 0 && val < 4096);

  auto const arr =
      std::to_array<std::uint8_t>({0b01000000U, std::uint8_t(val >> 4), std::uint8_t(val << 4)});

  return transfer(arr);
}

auto MCP4725::transfer(std::span<std::uint8_t const> arr) const noexcept -> std::error_code {
  auto const err = write(native_handle(), arr.data(), arr.size());

  if (err != ssize_t(arr.size()))
    return errno_to_err_code(errno);

  return {};
}

} // namespace ilanta
