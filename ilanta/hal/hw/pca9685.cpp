#include "ilanta/hal/hw/pca9685.hpp"
#include "ilanta/util/errors.hpp"

#include <spdlog/spdlog.h>

namespace ilanta {

PCA9685::PCA9685(I2C* const bus, std::uint16_t const addr) : bus_(bus), addr_(addr) {
  spdlog::info("Constructing PCA9685");

  [[unlikely]] if (!bus) err_log_throw("Invalid I2C bus");
}

auto PCA9685::addr() const noexcept -> std::uint16_t { return addr_; }
auto PCA9685::addr(std::uint16_t const addr) noexcept -> void { addr_ = addr; }

} // namespace ilanta
