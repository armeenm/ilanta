#pragma once

#include <gpiod.hpp>
#include <string_view>
#include <utility>

namespace ilanta {

auto request_output(gpiod::line const& l, std::string_view consumer, bool invert, int default_val)
    -> void;

template <typename Line>
auto inline request_output(Line&& l, std::string_view consumer, bool invert) {
  return request_output(std::forward<Line>(l), consumer, invert, 0);
}

template <typename Line> auto inline request_output(Line&& l, std::string_view consumer) {
  return request_output(std::forward<Line>(l), consumer, false, 0);
}

} // namespace ilanta
