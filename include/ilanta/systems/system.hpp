#pragma once

#include <concepts>
#include <type_traits>

#include "ilanta/util/generics.hpp"

namespace ilanta {

template <typename T>
class System;
enum class Mode { Disabled, Autonomous, Teleoperated };

template <typename T>
concept SystemC = requires(T a) {
  { a.init(Mode{}) }
  ->std::same_as<void>;

  { a.periodic(Mode{}) }
  ->std::same_as<void>;

  { a.template periodic<Mode{}>() }
  ->std::same_as<void>;

  requires IsBase<System<T>, T>;
};

template <typename T>
class System {
 public:
  template <Mode m>
  auto periodic() -> void {
    underlying().template periodic<m>();
  }

  auto periodic(Mode m) -> void { underlying().periodic(m); }

 private:
  friend T;
  System() = default;

  auto constexpr underlying() -> T& { return static_cast<T&>(*this); }
};

class Foo : System<Foo> {
 public:
  void init(Mode m);
  template <Mode m>
  void periodic() {}
  void periodic(Mode m);
};

using Y = System<Foo>;

template <SystemC S>
void bar(S s) {}

void baz() {
  auto foo = Foo{};
  bar<Foo>(foo);
}

}  // namespace ilanta
