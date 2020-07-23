#pragma once

/*
 * NOTE: currently unused; prefer I2CDevice
 */

#include <cerrno>
#include <concepts>
#include <cstdint>
#include <fcntl.h>
#include <filesystem>
#include <optional>
#include <span>
#include <spdlog/spdlog.h>
#include <system_error>
#include <vector>

#include "ilanta/util/errors.hpp"
#include "ilanta/util/result.hpp"

namespace ilanta {

namespace detail {
extern "C" {
#include "ilanta/third_party/smbus.h"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
}
} // namespace detail

class I2C {
public:
  struct Info {
    std::filesystem::path path;
    unsigned long funcs;
  };

  enum Flag : std::uint16_t {
    READ = I2C_M_RD,
    ADDR_TEN = I2C_M_TEN,
    NO_RD_ACK = I2C_M_NO_RD_ACK,
    IGNORE_NAK = I2C_M_IGNORE_NAK,
    NOSTART = I2C_M_NOSTART,
    STOP = I2C_M_STOP
  };

  using Message = detail::i2c_msg;
  // struct i2c_msg { __u16 addr, flags, len; __u8* buf } //

  [[nodiscard]] static auto find_buses() noexcept -> std::vector<std::filesystem::path>;

  [[nodiscard]] I2C(std::filesystem::path const&);

  I2C(I2C const&) = delete;
  I2C(I2C&&) noexcept = default;

  auto operator=(I2C const&) -> I2C& = delete;
  auto operator=(I2C &&) -> I2C& = default;

  ~I2C();

  [[nodiscard]] auto info() const noexcept -> Info;
  [[nodiscard]] auto fd() const noexcept -> int;

  [[nodiscard]] auto find_devs() const noexcept -> std::vector<std::uint16_t>;

  // Raw transfer //
  auto transfer(std::span<Message> data) const noexcept -> std::error_code;

  // Write //
  auto write(std::uint16_t addr, std::span<std::uint8_t const> bytes) const noexcept
      -> std::error_code;
  auto write(std::uint16_t addr, std::uint8_t bytes...) const noexcept -> std::error_code;

  // Read //
  auto read(std::uint16_t addr, std::span<std::uint8_t const> in,
            std::span<std::uint8_t> out) const noexcept -> std::error_code;

  [[nodiscard]] auto read(std::uint16_t addr, std::size_t read_count,
                          std::span<std::uint8_t const> bytes) const noexcept
      -> Result<std::vector<std::uint8_t>, std::error_code>;

  [[nodiscard]] auto read(std::uint16_t addr, std::size_t read_count,
                          std::uint8_t bytes...) const noexcept
      -> Result<std::vector<std::uint8_t>, std::error_code>;

  [[nodiscard]] auto read_byte(std::uint16_t addr,
                               std::span<std::uint8_t const> bytes) const noexcept
      -> Result<std::uint8_t, std::error_code>;

  [[nodiscard]] auto read_byte(std::uint16_t addr, std::uint8_t bytes...) const noexcept
      -> Result<std::uint8_t, std::error_code>;

private:
  Info info_;
  int fd_;
};

} // namespace ilanta
