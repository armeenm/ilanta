#pragma once

#include "ilanta/util/result.hpp"
#include <cstdint>
#include <filesystem>
#include <system_error>
#include <vector>

namespace ilanta {

namespace detail {
extern "C" {
#include "ilanta/third_party/smbus.h"
#include <linux/i2c-dev.h>
}
} // namespace detail

/**
 * This class is NOT thread-safe. Create instances per-thread.
 * Linux will manage synchronization.
 */
class I2CDevice {
public:
  [[nodiscard]] static auto find_buses() noexcept -> std::vector<std::filesystem::path>;

  [[nodiscard]] I2CDevice(unsigned int bus_num);
  [[nodiscard]] I2CDevice(std::filesystem::path const&);

  I2CDevice(I2CDevice const&) = default;
  I2CDevice(I2CDevice&&) noexcept = default;

  auto operator=(I2CDevice const&) -> I2CDevice& = default;
  auto operator=(I2CDevice&&) noexcept -> I2CDevice& = default;

  ~I2CDevice();

  [[nodiscard]] auto path() const noexcept -> std::filesystem::path;
  [[nodiscard]] auto native_handle() const noexcept -> int;
  [[nodiscard]] auto funcs() const noexcept -> unsigned long;

  [[nodiscard]] auto find_devs() const noexcept -> std::vector<std::uint16_t>;

  [[nodiscard]] auto addr(std::uint16_t) const noexcept -> std::error_code;

  [[nodiscard]] auto read_byte() const noexcept -> Result<std::uint8_t, std::error_code>;
  [[nodiscard]] auto read_byte(std::uint8_t cmd) const noexcept
      -> Result<std::uint8_t, std::error_code>;

  [[nodiscard]] auto read_word(std::uint8_t cmd) const noexcept
      -> Result<std::uint16_t, std::error_code>;

  auto write_byte(std::uint8_t val) const noexcept -> std::error_code;
  auto write_byte(std::uint8_t cmd, std::uint8_t val) const noexcept -> std::error_code;

  auto write_word(std::uint8_t cmd, std::uint16_t val) const noexcept -> std::error_code;

private:
  std::filesystem::path path_;
  int fd_;
  unsigned long funcs_;
};

} // namespace ilanta
