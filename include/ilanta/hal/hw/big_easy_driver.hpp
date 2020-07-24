#pragma once

#include <array>
#include <chrono>
#include <gpiod.hpp>
#include <optional>
#include <string_view>
#include <thread>

#include "ilanta/hal/logic_level.hpp"

namespace ilanta {

using gpiod::line;

class BigEasyDriver {
 public:
  // TODO: check directions
  enum Direction : int { Forward, Reverse };

  struct Pins {
    line step, dir;
    std::optional<line> en = std::nullopt;
    std::optional<line> rst = std::nullopt;
    std::optional<line> sleep = std::nullopt;
    std::optional<std::array<line, 3>> ms = std::nullopt;
  };

  BigEasyDriver(Pins const&, std::string_view consumer_name, LogicLevel reverse);
  BigEasyDriver(Pins const& pins, std::string_view consumer_name)
      : BigEasyDriver{pins, consumer_name, LogicLevel::High} {}

  BigEasyDriver(BigEasyDriver const&) = delete;
  BigEasyDriver(BigEasyDriver&&) noexcept = default;

  auto operator=(BigEasyDriver const&) -> BigEasyDriver& = delete;
  auto operator=(BigEasyDriver &&) -> BigEasyDriver& = default;

  ~BigEasyDriver() = default;

  [[nodiscard]] auto direction() const noexcept -> Direction;
  auto direction(Direction) const noexcept -> void;

  /*
   * It is HIGHLY recommended to set a real-time scheduling policy for the current process to get
   * accurate sleeps for the step(...) functions
   */
  auto step() const noexcept -> void { return step(20000); }
  auto step(long delay_ns) const noexcept -> void;

 private:
  Pins pins_;
};

}  // namespace ilanta
