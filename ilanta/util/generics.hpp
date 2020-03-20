#pragma once

#include <type_traits>

#define ILANTA_ALIAS_TEMPLATE_FUNC(new_f, old_f, attr)                                             \
  template <typename... Ts> attr inline auto new_f(Ts&&... args) {                                 \
    return old_f(std::forward<Ts>(args)...);                                                       \
  }

namespace ilanta {

template <typename T> concept Enum = std::is_enum_v<T>;

template <Enum E> constexpr auto underlying_cast(E e) {
  return static_cast<typename std::underlying_type_t<E>>(e);
}

} // namespace ilanta
