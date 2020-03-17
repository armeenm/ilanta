#pragma once

#include <spdlog/fmt/bundled/core.h>
#include <spdlog/spdlog.h>

#include <exception>
#include <gpiod.hpp>
#include <string_view>

#include "ilanta/util/strify.hpp"

#define ILANTA_GPIO_OUTPUT_THROW(l, ...)                                     \
  do {                                                                       \
    auto success =                                                           \
        ilanta::request_output(l, CONSUMER_NAME __VA_OPT__(, ) __VA_ARGS__); \
    if (!success)                                                            \
      throw std::runtime_error(fmt::format(                                  \
          "Failed to request " ILANTA_STRIFY(l) " line; offset {}",          \
          (l).offset()));                                                    \
  } while (0)

namespace ilanta {

auto request_output(gpiod::line const& l, std::string_view consumer,
                    bool invert = false, int default_val = 0) -> bool;

}  // namespace ilanta
