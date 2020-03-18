#pragma once

#include <cerrno>
#include <cstdint>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <type_traits>
#include <vector>

#include "ilanta/util/errors.hpp"

namespace ilanta {

namespace detail {
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <unistd.h>
} // namespace detail

class I2C {
public:
  struct Info {
    std::filesystem::path path;
  };

  enum class Flag {
    RD_EN = I2C_M_RD,
    ADDR_TEN = I2C_M_TEN,
    NO_RD_ACK = I2C_M_NO_RD_ACK,
    IGNORE_NAK = I2C_M_IGNORE_NAK
  };

  using Message = detail::i2c_msg;

  [[nodiscard]] static auto find() -> std::vector<Info>;

  template <typename T> I2C(T&& path) : info_{std::forward<T>(path)} {
    static_assert(std::is_convertible_v<std::remove_cvref_t<T>, std::filesystem::path>,
                  "'path' argument must be convertible to std::filesystem::path");

    spdlog::info("Constructing I2C");

    fd_ = detail::open(info_.path.c_str(), O_RDWR);
    if (fd_ < 0)
      err_log_throw<std::runtime_error>("Failed to open file: {}", strerror(errno));
  }

  I2C(I2C const&) = delete;
  I2C(I2C&&) noexcept = default;

  auto operator=(I2C const&) -> I2C& = delete;
  auto operator=(I2C &&) -> I2C& = default;

  ~I2C();

  [[nodiscard]] auto info() const noexcept -> Info;
  [[nodiscard]] auto fd() const noexcept -> int;

  auto transfer(std::vector<I2C::Message>&) const noexcept -> bool;

private:
  Info info_;
  int fd_;
};

} // namespace ilanta
