#pragma once

#include <type_traits>

#define ALIAS_TEMPLATE_FUNC(new_f, old_f, ...)                                                     \
  template <typename... Ts> __VA_ARGS__ inline auto new_f(Ts&&... args) {                          \
    return old_f(std::forward<Ts>(args)...);                                                       \
  }

namespace ilanta {

template <typename E> constexpr auto underlying_cast(E e) {
  return static_cast<typename std::underlying_type_t<E>>(e);
}

} // namespace ilanta
