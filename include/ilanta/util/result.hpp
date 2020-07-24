#pragma once

#include <concepts>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <variant>

namespace ilanta {

template <typename T, typename E>
class Result : public std::variant<T, E> {
 public:
  using ok_type = T;
  using err_type = E;
  using variant_type = std::variant<T, E>;

  [[nodiscard]] Result(T ok) : variant_type{std::move(ok)} {}
  [[nodiscard]] Result(E err) : variant_type{std::move(err)} {}

  [[nodiscard]] explicit constexpr operator bool() const noexcept { return is_err(); }

  [[nodiscard]] auto constexpr is_ok() const noexcept -> bool { return this->index() == 0; }
  [[nodiscard]] auto constexpr is_err() const noexcept -> bool { return this->index() == 1; }

  [[nodiscard]] auto constexpr unwrap() const -> T {
    return expect("Called `Result::unwrap()` while in error state.");
  }

  [[nodiscard]] auto constexpr expect(std::string_view msg) const -> T {
    if (is_err()) throw std::logic_error{msg.data()};

    return std::get<0>(*this);
  }

  template <typename Visitor>
  auto constexpr visit(Visitor&& visitor) const {
    return std::visit(visitor, *static_cast<variant_type const*>(this));
  }
};

}  // namespace ilanta
