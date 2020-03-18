#include "ilanta/io/i2c.hpp"
#include <algorithm>
#include <string_view>

namespace ilanta {

auto I2C::find_bus() noexcept -> std::vector<I2C::Info> {
  auto constexpr DIR_PREFIX = "/dev/";
  auto constexpr FILE_PREFIX = std::string_view{"i2c-"};

  auto ret = std::vector<I2C::Info>();

  for (auto const& entry : std::filesystem::directory_iterator{DIR_PREFIX})
    if (std::strncmp(entry.path().c_str(), FILE_PREFIX.data(), FILE_PREFIX.size()) == 0)
      ret.push_back({entry.path()});

  return ret;
}

I2C::~I2C() { detail::close(fd_); }

auto I2C::info() const noexcept -> I2C::Info { return info_; }
auto I2C::fd() const noexcept -> int { return fd_; }

// TODO: Eventually figure out 10-bit addressing...
auto I2C::find_dev() const noexcept -> std::vector<std::uint16_t> {
  auto constexpr MIN_ADDR_7 = 0x8;
  auto constexpr MAX_ADDR_7 = 0x78;

  auto ret = std::vector<std::uint16_t>();

  for (std::uint16_t addr = MIN_ADDR_7; addr < MAX_ADDR_7; ++addr) {
    auto data = std::vector<I2C::Message>{{.addr = addr, .flags = 0, .len = 0, .buf = nullptr}};

    auto success = transfer(data);
    if (success)
      ret.push_back(addr);
  }

  return ret;
}

auto I2C::transfer(std::vector<I2C::Message>& data) const noexcept -> bool {
  assert(data.size() <= UINT32_MAX);

  auto const ioctl_data =
      detail::i2c_rdwr_ioctl_data{data.data(), static_cast<detail::__u32>(data.size())};

  if (detail::ioctl(fd_, I2C_RDWR, &ioctl_data) < 0) {
    spdlog::error("Failed to transfer I2C data: {}", strerror(errno));
    return false;
  }

  return true;
}

} // namespace ilanta
