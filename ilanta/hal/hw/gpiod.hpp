#pragma once

#include <gpiod.hpp>
#include <string_view>

namespace ilanta {

auto request_output(gpiod::line const& l, std::string_view consumer, bool invert = false,
                    int default_val = 0) -> void;

} // namespace ilanta
