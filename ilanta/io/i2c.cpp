#include "ilanta/io/i2c.h"
#include "ilanta/util/errors.hpp"

#include <cerrno>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <spdlog/fmt/bundled/core.h>
#include <spdlog/spdlog.h>
#include <sys/ioctl.h>
#include <system_error>
#include <unistd.h>

namespace ilanta {

I2C::I2C(I2CInfo const& info) {
  spdlog::info("Constructing I2C");

  fd_ = open(info.path.c_str(), O_RDWR);
  if (fd_ < 0) {
    spdlog::error("Failed to open file: {}", strerror(errno));
    throw std::filesystem::filesystem_error("Failed to open file", info.path,
                                            ilanta::errno_to_err_code(errno));
  }

  addr(addr_);
}

I2C::~I2C() { close(fd_); }

auto I2C::fd() -> int& { return fd_; }

auto I2C::addr() -> int { return addr_; }

auto I2C::addr(int const addr) -> void {
  if (ioctl(fd_, I2C_SLAVE, addr) < 0) {
    auto const err = fmt::format("Failed to set I2C address to {}: {}", addr, strerror(errno));
    spdlog::error(err);
    throw std::runtime_error(err);
  }

  addr_ = addr;
}

} // namespace ilanta
