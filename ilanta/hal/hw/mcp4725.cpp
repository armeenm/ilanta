#include "ilanta/hal/hw/mcp4725.hpp"

namespace ilanta {

MCP4725::MCP4725(std::string_view path) : fd_{path, O_WRONLY} {}

auto MCP4725::fd() const noexcept -> FDesc { return fd_; }

auto MCP4725::val(std::uint16_t const val) const noexcept -> std::error_code {
  assert(val < 4096);

  return fd_.write(val);
}

} // namespace ilanta
