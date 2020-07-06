#include "ilanta/hal/hw/pca9685.hpp"
#include "ilanta/util/errors.hpp"
#include "ilanta/util/generics.hpp"

#include <array>
#include <spdlog/spdlog.h>

namespace ilanta {

auto constexpr chan_reg_offset(std::uint8_t channel) -> std::uint8_t {
  return std::uint8_t(4U * (channel - 1U));
}

PCA9685::PCA9685(I2C& bus, std::uint16_t const addr) : bus_{&bus}, addr_{addr} {
  spdlog::debug("Constructing PCA9685");

  reset();
  freq(1000U);
}

auto PCA9685::reset() const noexcept -> void {
  bus_->write(addr_, Reg::Mode1, 0x04); // Normal mode
  bus_->write(addr_, Reg::Mode2, 0x04); // Push-pull
}

auto PCA9685::freq() const noexcept -> std::uint16_t { return freq_; }

auto PCA9685::freq(std::uint16_t const freq) noexcept -> std::error_code {

  auto const prescale = static_cast<std::uint8_t>((clock_freq / 4096.0 / freq) - 1.0);

  auto e = bus_->write(addr_, Reg::Mode1, 0x10);        // Sleep
  e = e ?: bus_->write(addr_, Reg::PreScale, prescale); // PWM multiplier
  e = e ?: bus_->write(addr_, Reg::Mode1, 0x80);        // Restart
  e = e ?: bus_->write(addr_, Reg::Mode2, 0x04);        // Push-pull

  if (!e)
    freq_ = freq;

  return e;
}

auto PCA9685::duty_cycle(std::uint8_t const channel) -> Result<std::uint16_t, std::error_code> {
  auto const offset = chan_reg_offset(channel);

  auto val = std::uint16_t{bus_->read_byte(addr_, Reg::Chan0OffH + offset).unwrap()};
  val &= 0xFU;
  val <<= 8U;
  val += bus_->read_byte(addr_, Reg::Chan0OffL + offset).unwrap();

  return val;
}

auto PCA9685::duty_cycle(std::uint8_t const channel, std::uint16_t const duty_cycle)
    -> std::error_code {

  auto const offset = chan_reg_offset(channel);

  auto send = [=, this](std::uint8_t reg, unsigned int val) {
    return bus_->write(addr_, std::uint8_t(reg + offset), std::uint8_t(val));
  };

  auto e = send(Reg::Chan0OnL, duty_cycle & 0xFFU);
  e = e ?: send(Reg::Chan0OnH, duty_cycle >> 8U);
  e = e ?: send(Reg::Chan0OffL, 0U);
  e = e ?: send(Reg::Chan0OffH, 0U);

  return e;
}

auto PCA9685::addr() const noexcept -> std::uint16_t { return addr_; }
auto PCA9685::addr(std::uint16_t const addr) noexcept -> void { addr_ = addr; }

} // namespace ilanta
