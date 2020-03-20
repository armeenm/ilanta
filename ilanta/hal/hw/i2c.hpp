#pragma once

#include <cassert>
#include <cerrno>
#include <concepts>
#include <cstdint>
#include <fcntl.h>
#include <filesystem>
#include <optional>
#include <span>
#include <spdlog/spdlog.h>
#include <vector>

#include "ilanta/util/errors.hpp"

namespace ilanta {

namespace detail {
extern "C" {
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <unistd.h>
}
} // namespace detail

class I2C {
public:
  struct Info {
    std::filesystem::path path;
  };

  enum Flag {
    READ = I2C_M_RD,
    ADDR_TEN = I2C_M_TEN,
    NO_RD_ACK = I2C_M_NO_RD_ACK,
    IGNORE_NAK = I2C_M_IGNORE_NAK,
    NOSTART = I2C_M_NOSTART,
    STOP = I2C_M_STOP
  };

  using Message = detail::i2c_msg;
  /* struct i2c_msg { __u16 addr, flags, len; __u8* buf } */

  [[nodiscard]] static auto find_buses() noexcept -> std::vector<Info>;

  template <typename T>
  [[nodiscard]] I2C(T&& path) requires std::convertible_to<T, std::filesystem::path>
      : info_{std::forward<T>(path)} {

    spdlog::info("Constructing I2C");

    fd_ = open(info_.path.c_str(), O_RDWR);
    if (fd_ < 0)
      err_log_throw("Failed to open file: {}", std::strerror(errno));
  }

  I2C(I2C const&) = delete;
  I2C(I2C&&) noexcept = default;

  auto operator=(I2C const&) -> I2C& = delete;
  auto operator=(I2C &&) -> I2C& = default;

  ~I2C();

  [[nodiscard]] auto info() const noexcept -> Info;
  [[nodiscard]] auto fd() const noexcept -> int;

  [[nodiscard]] auto find_devs() const noexcept -> std::vector<std::uint16_t>;

  auto transfer(std::span<Message> data) const noexcept -> bool;

private:
  Info info_;
  int fd_;
};

} // namespace ilanta
