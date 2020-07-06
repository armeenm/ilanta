#pragma once

#include "ilanta/util/format.hpp"
#include "ilanta/util/result.hpp"

#include <concepts>
#include <exception>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <system_error>

namespace ilanta {

template <typename E = std::runtime_error, typename... Ts>
inline auto err_log_throw(std::string_view str,
                          Ts&&... args) requires std::constructible_from<E, std::string> {

  auto const err = format(str, std::forward<Ts>(args)...);
  spdlog::error(err);
  throw E{err};
}

[[nodiscard]] inline auto errno_to_err_code(int const errno_code) -> std::error_code {
  return std::make_error_code(static_cast<std::errc>(errno_code));
}

} // namespace ilanta
