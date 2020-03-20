#pragma once

namespace ilanta {

enum class LogicLevel : int { LOW, HIGH };

inline auto operator!(LogicLevel& orig) -> LogicLevel {
  return (orig == LogicLevel::LOW) ? LogicLevel::HIGH : LogicLevel::LOW;
}

} // namespace ilanta
