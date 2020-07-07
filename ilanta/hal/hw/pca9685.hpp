#pragma once

#include "ilanta/hal/hw/smbus.hpp"
#include "ilanta/util/result.hpp"

#include <cstdint>

namespace ilanta {

class PCA9685 {
public:
  [[nodiscard]] PCA9685(SMBus& bus) : PCA9685{bus, 0x40} {};
  [[nodiscard]] PCA9685(SMBus&, std::uint16_t addr);

  PCA9685(PCA9685 const&) = delete;
  [[nodiscard]] PCA9685(PCA9685&&) noexcept = default;

  auto operator=(PCA9685 const&) -> PCA9685& = delete;
  auto operator=(PCA9685&&) noexcept -> PCA9685& = default;

  ~PCA9685() = default;

  auto reset() const noexcept -> void;

  [[nodiscard]] auto freq() const noexcept -> std::uint16_t;
  auto freq(std::uint16_t) noexcept -> std::error_code;

  [[nodiscard]] auto duty_cycle(std::uint8_t channel) -> Result<std::uint16_t, std::error_code>;
  auto duty_cycle(std::uint8_t channel, std::uint16_t duty_cycle) -> std::error_code;

  [[nodiscard]] auto addr() const noexcept -> std::uint16_t;
  auto addr(std::uint16_t) noexcept -> void;

private:
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

  auto constexpr static inline clock_freq = 25000000.0;

  SMBus* bus_;
  std::uint16_t addr_;
  std::uint16_t freq_;
};

} // namespace ilanta
