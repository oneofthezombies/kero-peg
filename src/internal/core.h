#ifndef KERO_PEG_INTERNAL_CORE_H
#define KERO_PEG_INTERNAL_CORE_H

#include <optional>
#include <ostream>
#include <variant>

namespace kero {
namespace peg {

template <typename T, typename E> class Result {
public:
  Result(T&& data) noexcept : var_{std::move(data)} {}
  Result(E&& error) noexcept : var_{std::move(error)} {}
  Result(Result&&) noexcept = default;
  ~Result() noexcept = default;
  auto operator=(Result&&) -> Result& = default;

  Result(const Result&) = delete;
  auto operator=(const Result&) -> Result& = delete;

  auto IsOk() const noexcept -> bool { return std::holds_alternative<T>(var_); }

  auto IsErr() const noexcept -> bool {
    return std::holds_alternative<E>(var_);
  }

  auto Ok() noexcept -> std::optional<T> {
    if (IsOk()) {
      return std::get<T>(std::move(var_));
    }

    return std::nullopt;
  }

  auto Err() noexcept -> std::optional<E> {
    if (IsErr()) {
      return std::get<E>(std::move(var_));
    }

    return std::nullopt;
  }

  template <typename F>
  auto AndThen(F&& f) noexcept -> decltype(f(std::declval<T>())) {
    if (IsOk()) {
      return f(std::get<T>(std::move(var_)));
    }

    return std::get<E>(std::move(var_));
  }

  template <typename F>
  auto OrElse(F&& f) noexcept -> decltype(f(std::declval<E>())) {
    if (IsErr()) {
      return f(std::get<E>(std::move(var_)));
    }

    return std::get<T>(std::move(var_));
  }

private:
  std::variant<T, E> var_;
};

template <typename T, typename E>
auto operator<<(std::ostream& os, Result<T, E>& result) -> std::ostream& {
  os << "Result{";
  if (result.IsOk()) {
    os << "Ok{";
    os << *result.Ok();
    os << "}";
  } else {
    os << "Err{";
    os << *result.Err();
    os << "}";
  }
  os << "}";
  return os;
}

template <typename E> class Result<void, E> {
public:
  Result() noexcept = default;
  Result(E&& error) noexcept : error_{std::move(error)} {}
  Result(Result&&) noexcept = default;
  ~Result() noexcept = default;
  auto operator=(Result&&) -> Result& = default;

  Result(const Result&) = delete;
  auto operator=(const Result&) -> Result& = delete;

  auto IsOk() const noexcept -> bool { return !error_; }

  auto IsErr() const noexcept -> bool { return !!error_; }

  auto Ok() noexcept -> void {}

  auto Err() noexcept -> std::optional<E> {
    if (IsErr()) {
      return error_;
    }

    return std::nullopt;
  }

  template <typename F> auto AndThen(F&& f) noexcept -> decltype(f()) {
    if (IsOk()) {
      return f();
    }

    return std::get<E>(std::move(error_));
  }

  template <typename F>
  auto OrElse(F&& f) noexcept -> decltype(f(std::declval<E>())) {
    if (IsErr()) {
      return f(std::get<E>(std::move(error_)));
    }

    return Result<void, E>{};
  }

private:
  std::optional<E> error_;
};

template <typename E>
auto operator<<(std::ostream& os, Result<void, E>& result) -> std::ostream& {
  os << "Result{";
  if (result.IsOk()) {
    os << "Ok{";
    os << "void";
    os << "}";
  } else {
    os << "Err{";
    os << *result.Err();
    os << "}";
  }
  os << "}";
  return os;
}

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_CORE_H
