#pragma once

#include <system_error>

namespace ilanta {

inline auto errno_to_err_code(int const errno_code) -> std::error_code {
  return std::make_error_code(static_cast<std::errc>(errno_code));
}

} // namespace ilanta
