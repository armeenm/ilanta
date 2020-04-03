#include "ilanta/hal/hw/i2c.hpp"
#include <algorithm>
#include <string_view>

namespace ilanta {

auto I2C::find_buses() noexcept -> std::vector<I2C::Info> {
  auto constexpr DIR_PREFIX = "/dev/";
  auto constexpr FILE_PREFIX = std::string_view{"i2c-"};

  auto ret = std::vector<Info>();

  for (auto const& entry : std::filesystem::directory_iterator{DIR_PREFIX})
    if (std::strncmp(entry.path().c_str(), FILE_PREFIX.data(), FILE_PREFIX.size()) == 0)
      ret.push_back({entry.path()});

  return ret;
}

I2C::~I2C() { detail::close(fd_); }

auto I2C::info() const noexcept -> I2C::Info { return info_; }
auto I2C::fd() const noexcept -> int { return fd_; }

// TODO: Eventually figure out 10-bit addressing...
auto I2C::find_devs() const noexcept -> std::vector<std::uint16_t> {
  auto constexpr MIN_ADDR_7 = 0x8;
  auto constexpr MAX_ADDR_7 = 0x77;

  auto ret = std::vector<std::uint16_t>();

  for (std::uint16_t addr = MIN_ADDR_7; addr <= MAX_ADDR_7; ++addr) {
    auto data = std::array{Message{.addr = addr, .flags = 0, .len = 0, .buf = nullptr},
                           Message{.addr = addr, .flags = READ, .len = 0, .buf = nullptr}};

    if (!transfer(data))
      ret.push_back(addr);
  }

  return ret;
}

auto I2C::transfer(std::span<I2C::Message> data) const noexcept -> std::error_code {
  assert(data.size() <= std::numeric_limits<std::uint32_t>::max());

  auto const ioctl_data =
      detail::i2c_rdwr_ioctl_data{data.data(), static_cast<detail::__u32>(data.size())};

  if (detail::ioctl(fd_, I2C_RDWR, &ioctl_data) < 0) {
    spdlog::error("Failed to transfer I2C data: {}", std::strerror(errno));
    return errno_to_err_code(errno);
  }

  return {};
}

auto I2C::send(std::uint16_t const addr, std::span<std::uint8_t> const data) const noexcept
    -> std::error_code {

  assert(data.size() <= std::numeric_limits<std::uint32_t>::max());

  auto msg = Message{
      .addr = addr, .flags = 0, .len = static_cast<detail::__u16>(data.size()), .buf = data.data()};

  return transfer({&msg, 1});
}

auto I2C::recv(std::uint16_t const addr, std::span<std::uint8_t> const data) const noexcept
    -> std::error_code {
  assert(data.size() <= std::numeric_limits<std::uint32_t>::max());

  auto msg = Message{.addr = addr,
                     .flags = READ,
                     .len = static_cast<detail::__u16>(data.size()),
                     .buf = data.data()};

  return transfer({&msg, 1});
}

} // namespace ilanta
