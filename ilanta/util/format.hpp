#pragma once

#include "ilanta/util/generics.hpp"

#if __has_include(<format>)
#include <format>
namespace ilanta {
ALIAS_TEMPLATE_FUNC(format, std::format, [[nodiscard]])
}
#else
#include <spdlog/spdlog.h>
namespace ilanta {
ALIAS_TEMPLATE_FUNC(format, fmt::v6::format, [[nodiscard]])
} // namespace ilanta
#endif
