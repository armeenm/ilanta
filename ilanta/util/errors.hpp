#pragma once

#include <spdlog/fmt/bundled/core.h>
#include <spdlog/spdlog.h>
#include <string_view>
#include <system_error>
#include <type_traits>

namespace ilanta {

template <typename E, typename... Ts>
inline auto err_log_throw(std::string_view format_str, Ts... args) {
  static_assert(std::is_constructible_v<E, const char*>,
                "Template parameter 'E' must be constructible with a string for error message");

  auto const err = fmt::format(format_str, std::forward<Ts>(args)...);
}

inline auto errno_to_err_code(int const errno_code) -> std::error_code {
  return std::make_error_code(static_cast<std::errc>(errno_code));
}

} // namespace ilanta
