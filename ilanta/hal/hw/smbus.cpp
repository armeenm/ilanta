#include "ilanta/hal/hw/smbus.hpp"
#include "ilanta/hal/hw/i2c.hpp"
#include "ilanta/util/errors.hpp"

#include <cerrno>

namespace ilanta {

auto SMBus::find_buses() noexcept -> std::vector<std::filesystem::path> {
  return I2C::find_buses();
}

SMBus::SMBus(std::filesystem::path const& path) : info_{std::move(path), 0UL} {
  fd_ = open(info_.path.c_str(), O_RDWR);

  if (fd_ < 0)
    err_log_throw("Failed to open file '{}' for SMBus device: {}", info_.path.string(),
                  std::strerror(errno));

  if (detail::ioctl(fd_, I2C_FUNCS, &info_.funcs) < 0)
    err_log_throw("Failed to retrieve functionality of I2C device '{}': {}", info_.path.string(),
                  std::strerror(errno));
}

SMBus::~SMBus() { detail::close(fd_); }

auto SMBus::info() const noexcept -> Info { return info_; }
auto SMBus::fd() const noexcept -> int { return fd_; }

auto SMBus::find_devs() const noexcept -> std::vector<std::uint16_t> {
  auto constexpr min_addr = 0x03;
  auto constexpr max_addr = 0x77;

  auto ret = std::vector<std::uint16_t>{};

  for (std::uint16_t addr = min_addr; addr <= max_addr; ++addr) {
    if (detail::ioctl(fd_, I2C_SLAVE, addr) < 0)
      continue;

    auto const res = [=, this] {
      if (info_.funcs & I2C_FUNC_SMBUS_QUICK)
        return detail::i2c_smbus_write_quick(fd_, I2C_SMBUS_READ);
      else
        return detail::i2c_smbus_read_byte(fd_);
    }();

    if (res >= 0)
      ret.emplace_back(addr);
  }

  return ret;
}

auto SMBus::addr(std::uint16_t const addr) const noexcept -> std::error_code {
  if (detail::ioctl(fd_, I2C_SLAVE, addr) < 0)
    return errno_to_err_code(errno);
  else
    return {};
}

auto SMBus::read_byte(std::uint16_t const adr) const noexcept
    -> Result<std::uint8_t, std::error_code> {
  auto const err = addr(adr);
  if (err)
    return err;

  auto const res = detail::i2c_smbus_read_byte(fd_);
  if (res < 0)
    return errno_to_err_code(res);
  else
    return static_cast<std::uint8_t>(res);
}

auto SMBus::read_byte(std::uint16_t const adr, std::uint8_t const cmd) const noexcept
    -> Result<std::uint8_t, std::error_code> {

  auto const err = addr(adr);
  if (err)
    return err;

  auto const res = detail::i2c_smbus_read_byte_data(fd_, cmd);
  if (res < 0)
    return errno_to_err_code(res);
  else
    return static_cast<std::uint8_t>(res);
}

auto SMBus::read_word(std::uint16_t const adr, std::uint8_t const cmd) const noexcept
    -> Result<std::uint16_t, std::error_code> {

  auto const err = addr(adr);
  if (err)
    return err;

  auto const res = detail::i2c_smbus_read_word_data(fd_, cmd);
  if (res < 0)
    return errno_to_err_code(res);
  else
    return static_cast<std::uint8_t>(res);
}

auto SMBus::write_byte(std::uint16_t const adr, std::uint8_t const val) const noexcept
    -> std::error_code {
  auto const err = addr(adr);
  if (err)
    return err;

  return errno_to_err_code(detail::i2c_smbus_write_byte(fd_, val));
}

auto SMBus::write_byte(std::uint16_t const adr, std::uint8_t cmd, std::uint8_t val) const noexcept
    -> std::error_code {

  auto const err = addr(adr);
  if (err)
    return err;

  return errno_to_err_code(detail::i2c_smbus_write_byte_data(fd_, cmd, val));
}

auto SMBus::write_word(std::uint16_t const adr, std::uint8_t cmd, std::uint16_t val) const noexcept
    -> std::error_code {

  auto const err = addr(adr);
  if (err)
    return err;

  return errno_to_err_code(detail::i2c_smbus_write_word_data(fd_, cmd, val));
}

} // namespace ilanta
