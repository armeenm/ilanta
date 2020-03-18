#pragma once

#include <filesystem>
#include <vector>

namespace ilanta {

// For now this will just store a path
struct I2CInfo {
  std::filesystem::path path;
};

auto i2c_find_devs() -> std::vector<I2CInfo>;

class I2C {
public:
  I2C(I2CInfo const&);

  I2C(I2C const&) = delete;
  I2C(I2C&&) noexcept = default;

  auto operator=(I2C const&) -> I2C& = delete;
  auto operator=(I2C &&) -> I2C& = default;

  ~I2C();

  auto fd() -> int&;

  auto addr() -> int;
  auto addr(int addr) -> void;

  // auto transfer(

private:
  int fd_;
  int addr_ = 0;
};

} // namespace ilanta
