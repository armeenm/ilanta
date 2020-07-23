#pragma once

namespace ilanta {

enum class LogicLevel : int { Low, High };

inline auto operator!(LogicLevel& orig) -> LogicLevel {
  return (orig == LogicLevel::Low) ? LogicLevel::High : LogicLevel::Low;
}

} // namespace ilanta
