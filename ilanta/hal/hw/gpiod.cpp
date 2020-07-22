#include "ilanta/hal/hw/gpiod.hpp"
#include "ilanta/util/errors.hpp"

#include <spdlog/spdlog.h>

namespace ilanta {

auto request_output(gpiod::line const& l, std::string_view consumer, bool const invert,
                    int const default_val) -> void {

  spdlog::debug("Requesting line '{}' with offset {}", l.name(), l.offset());

  if (l.is_used())
    spdlog::warn("Line {} is in use by consumer '{}'", l.offset(), l.consumer());

  l.request({consumer.data(), gpiod::line_request::DIRECTION_OUTPUT,
             invert ? gpiod::line_request::FLAG_ACTIVE_LOW : 0},
            default_val);

  if (!l.is_requested())
    err_log_throw("Failed to request line {}", l.offset());

  spdlog::debug("Successfully requested line {}", l.offset());
}

} // namespace ilanta
