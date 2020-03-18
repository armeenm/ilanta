#pragma once

#include "ilanta/io/logic_level.hpp"
#include <array>
#include <gpiod.hpp>
#include <optional>

namespace ilanta {

using gpiod::line;

class BigEasyDriver {
public:
  // TODO: check directions
  enum Direction : int { FORWARD, REVERSE };

  struct Pins {
    line step, dir;
    std::optional<line> en = std::nullopt;
    std::optional<line> rst = std::nullopt;
    std::optional<line> sleep = std::nullopt;
    std::optional<std::array<line, 3>> ms = std::nullopt;
  };

  BigEasyDriver(Pins&&, LogicLevel reverse = LogicLevel::HIGH);

  BigEasyDriver(BigEasyDriver const&) = delete;
  BigEasyDriver(BigEasyDriver&&) noexcept = default;

  auto operator=(BigEasyDriver const&) -> BigEasyDriver& = delete;
  auto operator=(BigEasyDriver &&) -> BigEasyDriver& = default;

  ~BigEasyDriver() = default;

  auto move(Direction, unsigned int steps) noexcept -> void;

private:
  Pins pins_;
};

} // namespace ilanta
