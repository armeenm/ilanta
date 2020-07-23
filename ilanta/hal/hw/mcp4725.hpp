#pragma once

#include "ilanta/linux/fdesc.hpp"

#include <cstdint>
#include <string_view>
#include <system_error>

namespace ilanta {

class MCP4725 {
public:
  [[nodiscard]] explicit MCP4725(std::string_view path);

  MCP4725(MCP4725 const&) = default;
  [[nodiscard]] MCP4725(MCP4725&&) noexcept = default;

  auto operator=(MCP4725 const&) -> MCP4725& = default;
  auto operator=(MCP4725&&) noexcept -> MCP4725& = default;

  ~MCP4725() = default;

  [[nodiscard]] auto fd() const noexcept -> FDesc;

  auto val(std::uint16_t) const noexcept -> std::error_code;

private:
  FDesc fd_;
};

} // namespace ilanta
