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

auto constexpr chan_reg_offset(std::uint8_t channel) -> std::uint8_t {
  return std::uint8_t(4U * channel);
}

PCA9685::PCA9685(SMBus&& bus, std::uint16_t const addr) : bus_{std::move(bus)}, addr_{addr} {
  reset();
  freq(1000U);
}

auto PCA9685::reset() const noexcept -> void {
  bus_.write_byte(Reg::Mode1, 0x04); // Normal mode
  bus_.write_byte(Reg::Mode2, 0x04); // Push-pull
}

auto PCA9685::freq() const noexcept -> std::uint16_t { return freq_; }

auto PCA9685::freq(std::uint16_t const freq) noexcept -> std::error_code {

  auto const prescale = static_cast<std::uint8_t>((clock_freq / 4096.0 / freq) - 1.0);

  auto e = bus_.write_byte(Reg::Mode1, 0x10);        // Sleep
  e = e ?: bus_.write_byte(Reg::PreScale, prescale); // PWM multiplier
  e = e ?: bus_.write_byte(Reg::Mode1, 0x80);        // Restart
  e = e ?: bus_.write_byte(Reg::Mode2, 0x04);        // Push-pull

  if (!e)
    freq_ = freq;

  return e;
}

auto PCA9685::duty_cycle(std::uint8_t const channel) -> Result<std::uint16_t, std::error_code> {
  auto const offset = chan_reg_offset(channel);

  auto val = std::uint16_t{bus_.read_byte(Reg::Chan0OffH + offset).unwrap()};
  val &= 0xFU;
  val <<= 8U;
  val += bus_.read_byte(Reg::Chan0OffL + offset).unwrap();

  return val;
}

auto PCA9685::duty_cycle(std::uint8_t const channel, std::uint16_t const duty_cycle)
    -> std::error_code {

  assert(duty_cycle >= 0U && duty_cycle < 4096U);

  auto const offset = chan_reg_offset(channel);

  spdlog::debug("Duty cycle: {}", duty_cycle);

  auto send = [=, this](std::uint8_t reg, unsigned int val) {
    return bus_.write_byte(std::uint8_t(reg + offset), std::uint8_t(val));
  };

  auto e = send(Reg::Chan0OffL, duty_cycle & 0xFFU);
  e = e ?: send(Reg::Chan0OffH, duty_cycle >> 8U);
  e = e ?: send(Reg::Chan0OnL, 0U);
  e = e ?: send(Reg::Chan0OnH, 0U);

  return e;
}

auto PCA9685::addr() const noexcept -> std::uint16_t { return addr_; }
auto PCA9685::addr(std::uint16_t const addr) noexcept -> void { addr_ = addr; }

} // namespace ilanta
