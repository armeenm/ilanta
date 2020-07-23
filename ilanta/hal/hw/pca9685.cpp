#include "ilanta/hal/hw/pca9685.hpp"

namespace ilanta {

auto constexpr inline clock_freq = 25000000.0;

PCA9685::PCA9685(std::string_view path) : fd_{path, O_RDWR} {}

auto PCA9685::reset() const noexcept -> std::error_code {}

auto PCA9685::freq(std::uint16_t const freq) const noexcept -> std::error_code {}

auto PCA9685::duty_cycle(std::uint8_t const channel) -> Result<std::uint16_t, std::error_code> {}

auto PCA9685::duty_cycle(std::uint8_t const channel, std::uint16_t const duty_cycle)
    -> std::error_code {}

} // namespace ilanta
