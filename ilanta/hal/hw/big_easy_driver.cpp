#include "ilanta/hal/hw/big_easy_driver.hpp"

#include "ilanta/hal/hw/gpiod.hpp"

#include <chrono>
#include <exception>
#include <spdlog/spdlog.h>
#include <thread>

namespace ilanta {

BigEasyDriver::BigEasyDriver(Pins&& pins, std::string_view consumer_name, LogicLevel const reverse)
    : pins_(std::move(pins)) {
  spdlog::info("Constructing BigEasyDriver");

  auto const setup_l = [&](gpiod::line const& l, auto&&... ts) {
    request_output(l, consumer_name, std::forward<decltype(ts)>(ts)...);
  };

  setup_l(pins_.step);

  setup_l(pins_.dir, reverse == LogicLevel::LOW);

  if (pins_.en)
    setup_l(*pins_.en, true);

  if (pins_.rst)
    setup_l(*pins_.rst, true);

  if (pins_.sleep)
    setup_l(*pins_.sleep, true);

  if (pins_.ms)
    for (auto const& pin : *pins_.ms)
      setup_l(pin);
}

/**
 * Blocking function which moves the stepper motor a given number of steps in the specified
 * direction. Bitbangs a square wave; RT may be necessary for smooth movement of the stepper
 */
auto BigEasyDriver::move(Direction const dir, unsigned int const steps) noexcept -> void {
  using namespace std::chrono_literals;

  auto constexpr PERIOD_HALF = 50ns;

  // Set desired direction
  pins_.dir.set_value(dir);

  for (unsigned int i = 0; i < steps; ++i) {
    pins_.step.set_value(1);
    std::this_thread::sleep_for(PERIOD_HALF);
    pins_.step.set_value(0);
    std::this_thread::sleep_for(PERIOD_HALF);
  }
}

} // namespace ilanta
