#include "ilanta/hal/hw/i2c.hpp"
#include <algorithm>
#include <cassert>
#include <string_view>
#include <fmt/core.h>

namespace ilanta {

auto I2C::find_buses() noexcept -> std::vector<I2C::Info> {
  using namespace std::literals;

  auto constexpr dir_prefix = "/dev/"sv;
  auto constexpr file_prefix = "i2c-"sv;

  auto ret = std::vector<Info>{};

  for (auto const& entry : std::filesystem::directory_iterator{dir_prefix}) {
    auto const cmp = std::strncmp(entry.path().filename().c_str(),
                                 file_prefix.data(),
                                 file_prefix.size());

    if (cmp == 0)
      ret.emplace_back(entry.path());
  }

  return ret;
}

I2C::~I2C() { detail::close(fd_); }

auto I2C::info() const noexcept -> I2C::Info { return info_; }
auto I2C::fd() const noexcept -> int { return fd_; }

// TODO: Eventually figure out 10-bit addressing...
auto I2C::find_devs() const noexcept -> std::vector<std::uint16_t> {
  auto constexpr MIN_ADDR_7 = 0x8;
  auto constexpr MAX_ADDR_7 = 0x77;

  auto ret = std::vector<std::uint16_t>{};

  for (std::uint16_t addr = MIN_ADDR_7; addr <= MAX_ADDR_7; ++addr) {
    auto data = std::array{Message{.addr = addr, .flags = 0, .len = 0, .buf = nullptr},
                           Message{.addr = addr, .flags = READ, .len = 0, .buf = nullptr}};

    if (!transfer(data))
      ret.emplace_back(addr);
  }

  return ret;
}

auto I2C::transfer(std::span<I2C::Message> const data) const noexcept -> std::error_code {
  assert(data.size() <= std::numeric_limits<std::uint32_t>::max());

  auto const ioctl_data =
      detail::i2c_rdwr_ioctl_data{data.data(), static_cast<detail::__u32>(data.size())};

  if (detail::ioctl(fd_, I2C_RDWR, &ioctl_data) < 0) {
    spdlog::error("Failed to transfer I2C data: {}", std::strerror(errno));
    return errno_to_err_code(errno);
  }

  return {};
}

auto I2C::send(std::uint16_t const addr, std::span<std::uint8_t const> const data) const noexcept
    -> std::error_code {

  assert(data.size() <= std::numeric_limits<std::uint32_t>::max());

  auto msg = Message{.addr = addr,
                     .flags = 0,
                     .len = static_cast<detail::__u16>(data.size()),
                     .buf = const_cast<std::uint8_t*>(data.data())};

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

auto I2C::send_recv(std::uint16_t const addr, std::span<std::uint8_t> const data) const noexcept
    -> std::error_code {

  auto const err = send(addr, data);

  return err ? err : recv(addr, data);
}

} // namespace ilanta
