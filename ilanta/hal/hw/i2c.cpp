#include "ilanta/hal/hw/i2c.hpp"

#include "ilanta/util/generics.hpp"

#include <algorithm>
#include <cassert>
#include <fmt/core.h>
#include <string_view>
#include <spdlog/spdlog.h>

namespace ilanta {

auto I2C::find_buses() noexcept -> std::vector<std::filesystem::path> {
  using namespace std::literals;

  auto constexpr dir_prefix = "/dev/"sv;
  auto constexpr file_prefix = "i2c-"sv;

  auto ret = std::vector<std::filesystem::path>{};

  for (auto const& entry: std::filesystem::directory_iterator{dir_prefix}) {
    auto const cmp =
        std::strncmp(entry.path().filename().c_str(), file_prefix.data(), file_prefix.size());

    if (cmp == 0)
      ret.emplace_back(entry);
  }

  return ret;
}

I2C::I2C(std::filesystem::path const& path) : info_{std::move(path), 0UL} {
  fd_ = open(info_.path.c_str(), O_RDWR);

  if (fd_ < 0)
    err_log_throw("Failed to open file '{}' for I2C device: {}",
      info_.path.string(), std::strerror(errno));

  if (detail::ioctl(fd_, I2C_FUNCS, &info_.funcs) < 0)
    err_log_throw("Failed to retrieve functionality of I2C device '{}': {}",
      info_.path.string(), std::strerror(errno));
}

I2C::~I2C() { detail::close(fd_); }

auto I2C::info() const noexcept -> I2C::Info { return info_; }
auto I2C::fd() const noexcept -> int { return fd_; }

// TODO: Eventually figure out 10-bit addressing...
auto I2C::find_devs() const noexcept -> std::vector<std::uint16_t> {
  auto constexpr min_addr = 0x03;
  auto constexpr max_addr = 0x77;

  auto ret = std::vector<std::uint16_t>{};

  for (std::uint16_t addr = min_addr; addr <= max_addr; ++addr) {
    spdlog::debug("Finding device at address {}", addr);

    if (detail::ioctl(fd_, I2C_SLAVE, addr) < 0) {
      spdlog::debug("Failed to set slave address to {}: {}", addr, std::strerror(errno));
      continue;
    }

    /*
    if (detail::i2c_smbus_write_quick(fd_, I2C_SMBUS_WRITE) < 0) {
      spdlog::debug("Could not write to {}: {}", addr, std::strerror(errno));
      continue;
    }
    */

    ret.emplace_back(addr);
  }

  return ret;
}

auto I2C::transfer(std::span<I2C::Message> const data) const noexcept -> std::error_code {
  assert(data.size() <= std::numeric_limits<std::uint32_t>::max());

  auto const ioctl_data =
      detail::i2c_rdwr_ioctl_data{data.data(), static_cast<detail::__u32>(data.size())};

  if (detail::ioctl(fd_, I2C_RDWR, &ioctl_data) < 0)
    return errno_to_err_code(errno);

  return {};
}

auto I2C::write(std::uint16_t const addr, std::span<std::uint8_t const> const bytes) const noexcept
    -> std::error_code {

  assert(bytes.size() <= std::numeric_limits<std::uint16_t>::max());

  auto msg = Message{.addr = addr,
                     .flags = 0,
                     .len = static_cast<detail::__u16>(bytes.size()),
                     .buf = const_cast<detail::__u8*>(bytes.data())};

  return transfer({&msg, 1});
}

auto I2C::write(std::uint16_t const addr, std::uint8_t bytes...) const noexcept -> std::error_code {
  return write(addr, {{bytes}});
}

auto I2C::read(std::uint16_t const addr, std::span<std::uint8_t const> const in,
               std::span<std::uint8_t> const out) const noexcept -> std::error_code {

  assert(in.size() <= std::numeric_limits<std::uint16_t>::max());

  auto const send = Message{.addr = addr,
                            .flags = 0,
                            .len = static_cast<detail::__u16>(in.size()),
                            .buf = const_cast<detail::__u8*>(in.data())};

  auto const recv = Message{.addr = addr,
                            .flags = READ,
                            .len = static_cast<detail::__u16>(out.size()),
                            .buf = out.data()};

  auto msgs = std::array{send, recv};

  return transfer(msgs);
}

auto I2C::read(std::uint16_t const addr, std::size_t const read_count,
               std::span<std::uint8_t const> const bytes) const noexcept
    -> Result<std::vector<std::uint8_t>, std::error_code> {

  auto out = std::vector<std::uint8_t>{};
  out.reserve(read_count);

  auto const err = read(addr, bytes, out);

  if (err)
    return err;
  else
    return out;
}

auto I2C::read(std::uint16_t const addr, std::size_t const read_count,
               std::uint8_t const bytes...) const noexcept
    -> Result<std::vector<std::uint8_t>, std::error_code> {

  auto out = std::vector<std::uint8_t>{};
  out.reserve(read_count);

  auto const err = read(addr, {{bytes}}, out);

  if (err)
    return err;
  else
    return out;
}

auto I2C::read_byte(std::uint16_t const addr,
                    std::span<std::uint8_t const> const bytes) const noexcept
    -> Result<std::uint8_t, std::error_code> {

  std::uint8_t ret;

  auto const err = read(addr, bytes, {&ret, 1});

  if (err)
    return err;
  else
    return ret;
}

auto I2C::read_byte(std::uint16_t const addr, std::uint8_t bytes...) const noexcept
    -> Result<std::uint8_t, std::error_code> {
  return read_byte(addr, {{bytes}});
}

} // namespace ilanta
