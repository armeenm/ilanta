#include "ilanta/linux/fdesc.hpp"

namespace ilanta {

FDesc::FDesc(std::string_view filename, int flags) {
  fd_ = open(filename.data(), flags);

  if (fd_ < 0) {
    auto const e = errno_to_err_code(errno);
    err_log_throw("Failed to open file descriptor: {} [{}]", e.message(), e.value());
  }
}

FDesc::FDesc(FDesc const& other) : fd_{dup(other.fd_)} {
  if (fd_ < 0) {
    auto const e = errno_to_err_code(errno);
    err_log_throw("Failed to duplicate file descriptor: {} [{}]", e.message(), e.value());
  }
}

FDesc::FDesc(FDesc&& other) noexcept : fd_{other.fd_} { other.open_ = false; }

auto FDesc::operator=(FDesc const& other) -> FDesc& {
  if (this != &other) {
    close(fd_);
    fd_ = dup(other.fd_);

    if (fd_ < 0) {
      auto const e = errno_to_err_code(errno);
      err_log_throw("Failed to duplicate file descriptor: {} [{}]", e.message(), e.value());
    }
  }

  return *this;
}

auto FDesc::operator=(FDesc&& other) noexcept -> FDesc& {
  if (this != &other) {
    close(fd_);
    fd_ = other.fd_;
    other.open_ = false;
  }

  return *this;
}

FDesc::~FDesc() {
  if (open_) {
    close(fd_);
    open_ = false;
  }
}

auto FDesc::fd() const noexcept -> int { return fd_; }

auto FDesc::path() const -> std::string {
  auto res = std::string{};
  res.reserve(PATH_MAX);

  auto const proc_fname = fmt::format("/proc/self/fd/{}", fd_);

  if (readlink(proc_fname.c_str(), res.data(), res.size()) < 0) {
    auto const e = errno_to_err_code(errno);
    err_log_throw("Failed to get fd path: {}  [{}]", e.message(), e.value());
  }

  return res;
}

auto FDesc::write_arr(std::span<std::uint8_t const> const arr) const noexcept -> std::error_code {
  if (::write(fd_, arr.data(), arr.size_bytes()) < 0) return errno_to_err_code(errno);

  return {};
}

auto FDesc::operator*() const noexcept -> int { return fd(); }

}  // namespace ilanta
