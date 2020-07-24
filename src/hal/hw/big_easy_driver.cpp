#include "ilanta/hal/hw/big_easy_driver.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <thread>

#include "ilanta/hal/hw/gpiod.hpp"
#include "ilanta/util/generics.hpp"

namespace ilanta {

BigEasyDriver::BigEasyDriver(Pins const& pins, std::string_view const consumer_name,
                             LogicLevel const reverse)
    : pins_(std::move(pins)) {
  auto const setup_l = [&](gpiod::line const& l, auto&&... ts) {
    request_output(l, consumer_name, std::forward<decltype(ts)>(ts)...);
  };

  setup_l(pins_.step);
  setup_l(pins_.dir, reverse == LogicLevel::Low);
  if (pins_.en) setup_l(*pins_.en, true);
  if (pins_.rst) setup_l(*pins_.rst, true);
  if (pins_.sleep) setup_l(*pins_.sleep, true);
  if (pins_.ms)
    for (auto const& pin : *pins_.ms) setup_l(pin);
}

auto BigEasyDriver::direction() const noexcept -> Direction {
  return Direction{pins_.dir.get_value()};
}

auto BigEasyDriver::direction(Direction dir) const noexcept -> void {
  pins_.dir.set_value(underlying_cast(dir));
}

auto BigEasyDriver::step(long delay_ns) const noexcept -> void {
  auto const spec = timespec{0, delay_ns};

  pins_.step.set_value(1);
  nanosleep(&spec, nullptr);
  pins_.step.set_value(0);
  nanosleep(&spec, nullptr);
}

}  // namespace ilanta
