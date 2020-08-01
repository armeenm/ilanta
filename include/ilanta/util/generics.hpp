#pragma once

#include <concepts>
#include <type_traits>

#define ILANTA_ALIAS_FN(new_f, old_f, attr)  \
  template <typename... Ts>                  \
  attr auto inline new_f(Ts&&... args) {     \
    return old_f(std::forward<Ts>(args)...); \
  }

namespace ilanta {

template <typename T>
concept Enum = std::is_enum_v<T>;

template <typename T>
concept Arithmetic = std::floating_point<T> || std::integral<T>;

template <typename Base, typename Derived>
concept IsBase = std::is_base_of_v<Base, Derived>;

template <Enum E>
constexpr auto underlying_cast(E e) {
  return static_cast<typename std::underlying_type_t<E>>(e);
}

template <typename...>
constexpr std::false_type always_false{};

template <typename... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

struct Unit {};

}  // namespace ilanta
