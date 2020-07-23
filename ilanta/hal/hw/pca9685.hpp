#pragma once

#include "ilanta/linux/fdesc.hpp"
#include "ilanta/util/result.hpp"

#include <cstdint>
#include <system_error>
#include <utility>

namespace ilanta {

class PCA9685 {
public:
  [[nodiscard]] explicit PCA9685(std::string_view path);

  PCA9685(PCA9685 const&) = default;
  PCA9685(PCA9685&&) noexcept = default;

  auto operator=(PCA9685 const&) -> PCA9685& = default;
  auto operator=(PCA9685&&) noexcept -> PCA9685& = default;

  ~PCA9685() = default;

  auto reset() const noexcept -> std::error_code;

  auto freq(std::uint16_t) const noexcept -> std::error_code;

  [[nodiscard]] auto duty_cycle(std::uint8_t channel) -> Result<std::uint16_t, std::error_code>;
  auto duty_cycle(std::uint8_t channel, std::uint16_t duty_cycle) -> std::error_code;

private:
  FDesc fd_;
};

} // namespace ilanta
