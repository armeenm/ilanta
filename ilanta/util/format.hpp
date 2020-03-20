#pragma once

#include "ilanta/util/generics.hpp"

#if __has_include(<format>)
#include <format>
#else
#include <spdlog/spdlog.h>
#endif

namespace ilanta {

template <typename... Ts> inline auto format(std::string_view str, Ts&&... args) {
#if __has_include(<format>)
  return std::format(str, std::forward<Ts>(args)...);
#else
  return fmt::v6::format(str, std::forward<Ts>(args)...);
#endif
}

} // namespace ilanta
