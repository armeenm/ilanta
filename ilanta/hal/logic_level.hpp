#pragma once

namespace ilanta {

enum class LogicLevel : int { Low, High };

[[nodiscard]] auto inline operator!(LogicLevel& orig) -> LogicLevel {
  return (orig == LogicLevel::Low) ? LogicLevel::High : LogicLevel::Low;
}

}  // namespace ilanta
