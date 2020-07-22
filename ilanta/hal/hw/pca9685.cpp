#include "ilanta/hal/hw/pca9685.hpp"
#include "ilanta/util/errors.hpp"
#include "ilanta/util/generics.hpp"

#include <array>
#include <spdlog/spdlog.h>

namespace ilanta {

enum Reg : std::uint8_t {
  Mode1,
  Mode2,
  SubAdr1,
  SubAdr2,
  SubAdr3,
  AllCallAdr,
  Chan0OnL,
  Chan0OnH,
  Chan0OffL,
  Chan0OffH,
  AllChanOnL = 0xFA,
  AllChanOnH,
  AllChanOffL,
  AllChanOffH,
  PreScale,
  TestMode
};

enum Mode1Reg : std::uint8_t {
  AllCall = 0x01,
  Sub3 = 0x02,
  Sub2 = 0x04,
  Sub1 = 0x08,
  Sleep = 0x10,
  Ai = 0x20,
  Extclk = 0x40,
  Restart = 0x80
};

enum Mode2Reg : std::uint8_t {
  OutNE_0 = 0x01,
  OutNE_1 = 0x02,
  OutDrv = 0x04,
  OCH = 0x08,
  Invrt = 0x10
};

auto constexpr inline clock_freq = 25000000.0;

auto constexpr chan_reg_offset(std::uint8_t const channel) -> std::uint8_t {
  return std::uint8_t(4U * channel);
}

PCA9685::PCA9685(std::filesystem::path const& path, std::uint16_t const addr_in)
  : I2CDevice{std::move(path)} {

  auto e = addr(addr_in);
  if (e)
    err_log_throw("Failed to set address to {}: {} [{}]", addr_in, e.message(), e.value());

  e = reset();
  if (e)
    err_log_throw("Failed to reset device: {} [{}]", e.message(), e.value());
}

auto PCA9685::reset() const noexcept -> std::error_code {
  auto e = write_byte(Reg::Mode1, 0x04); // Normal mode
  e = e ?: write_byte(Reg::Mode2, 0x04); // Push-pull

  return e;
}

auto PCA9685::freq(std::uint16_t const freq) const noexcept -> std::error_code {
  auto const prescale = static_cast<std::uint8_t>((clock_freq / 4096.0 / freq) - 1.0);

  auto e = write_byte(Reg::Mode1, 0x10);        // Sleep
  e = e ?: write_byte(Reg::PreScale, prescale); // PWM multiplier
  e = e ?: write_byte(Reg::Mode1, 0x80);        // Restart
  e = e ?: write_byte(Reg::Mode2, 0x04);        // Push-pull

  return e;
}

auto PCA9685::duty_cycle(std::uint8_t const channel) -> Result<std::uint16_t, std::error_code> {
  assert(channel < 16U);

  auto const offset = chan_reg_offset(channel);

  auto val = std::uint16_t{read_byte(Reg::Chan0OffH + offset).unwrap()};
  val &= 0xFU;
  val <<= 8U;
  val += read_byte(Reg::Chan0OffL + offset).unwrap();

  return val;
}

auto PCA9685::duty_cycle(std::uint8_t const channel, std::uint16_t const duty_cycle)
    -> std::error_code {

  assert(duty_cycle >= 0U && duty_cycle < 4096U);
  assert(channel < 16U);

  auto const offset = chan_reg_offset(channel);

  auto send = [=, this](std::uint8_t reg, unsigned int val) {
    return write_byte(std::uint8_t(reg + offset), std::uint8_t(val));
  };

  auto e = send(Reg::Chan0OffL, duty_cycle & 0xFFU);
  e = e ?: send(Reg::Chan0OffH, duty_cycle >> 8U);
  e = e ?: send(Reg::Chan0OnL, 0U);
  e = e ?: send(Reg::Chan0OnH, 0U);

  return e;
}

} // namespace ilanta
