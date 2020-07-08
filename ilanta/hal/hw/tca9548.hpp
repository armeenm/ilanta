#pragma once

#include "ilanta/hal/hw/smbus.hpp"

#include <cstdint>

namespace ilanta {

class TCA9548 {
public:
  [[nodiscard]] TCA9548(SMBus&& bus) : TCA9548{std::move(bus), 0x70} {}
  [[nodiscard]] TCA9548(SMBus&& bus, std::uint16_t addr);

  TCA9548(TCA9548 const&) = delete;
  [[nodiscard]] TCA9548(TCA9548&&) noexcept = default;

  auto operator=(TCA9548 const&) -> TCA9548& = delete;
  auto operator=(TCA9548&&) noexcept -> TCA9548& = default;

  ~TCA9548() = default;

  auto port(std::uint8_t) const noexcept -> std::error_code;
  auto addr(std::uint16_t) const noexcept -> std::error_code;

private:
  SMBus bus_;
};

} // namespace ilanta
