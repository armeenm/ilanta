#pragma once

#include "ilanta/util/errors.hpp"
#include "ilanta/util/result.hpp"

#include <cerrno>
#include <fcntl.h>
#include <filesystem>
#include <fmt/core.h>
#include <linux/limits.h>
#include <span>
#include <string>
#include <system_error>
#include <unistd.h>

namespace ilanta {

class FDesc {
public:
  [[nodiscard]] FDesc(std::string_view filename, int flags);

  FDesc(FDesc const&);
  FDesc(FDesc&&) noexcept;

  auto operator=(FDesc const&) -> FDesc&;
  auto operator=(FDesc&&) noexcept -> FDesc&;

  ~FDesc();

  [[nodiscard]] auto fd() const noexcept -> int;
  [[nodiscard]] auto path() const -> std::string;

  auto write_arr(std::span<std::uint8_t const> arr) const noexcept -> std::error_code;

  template <typename T> auto write(T const& data) const noexcept -> std::error_code {
    return write_arr({reinterpret_cast<std::uint8_t const*>(&data), sizeof(T)});
  }

  [[nodiscard]] auto operator*() const noexcept -> int;

private:
  int fd_;
  bool open_ = true;
};

} // namespace ilanta
