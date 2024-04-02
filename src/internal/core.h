#ifndef KERO_PEG_INTERNAL_CORE_H
#define KERO_PEG_INTERNAL_CORE_H

#include <cassert>
#include <optional>
#include <ostream>

namespace kero {
namespace peg {

template <typename T, typename E> class Result {
public:
  Result(T&& data) noexcept : data_{std::move(data)} {}
  Result(E&& error) noexcept : error_{std::move(error)} {}
  Result(Result&&) noexcept = default;
  ~Result() noexcept = default;
  auto operator=(Result&&) -> Result& = default;

  Result(const Result&) = delete;
  auto operator=(const Result&) -> Result& = delete;

  auto IsOk() const noexcept -> bool { return data_.has_value(); }

  auto IsErr() const noexcept -> bool { return error_.has_value(); }

  auto Ok() noexcept -> std::optional<T> {
    if (IsOk()) {
      assert(data_.has_value());
      assert(!error_.has_value());
      auto data = std::move(data_);
      data_.reset();
      return data;
    }
    return std::nullopt;
  }

  auto Err() noexcept -> std::optional<E> {
    if (IsErr()) {
      assert(!data_.has_value());
      assert(error_.has_value());
      auto error = std::move(error_);
      error_.reset();
      return error;
    }
    return std::nullopt;
  }

  template <typename F>
  auto AndThen(F&& f) noexcept -> decltype(f(std::declval<T>())) {
    static_assert(std::is_invocable_v<F, T>,
                  "Function must be invocable with T");
    if (IsOk()) {
      assert(data_.has_value());
      assert(!error_.has_value());
      return std::forward<F>(f)(std::move(*data_));
    }
    assert(!data_.has_value());
    assert(error_.has_value());
    return decltype(f(std::declval<T>())){std::move(*error_)};
  }

  template <typename F>
  auto OrElse(F&& f) noexcept -> decltype(f(std::declval<E>())) {
    static_assert(std::is_invocable_v<F, E>,
                  "Function must be invocable with E");
    if (IsErr()) {
      assert(!data_.has_value());
      assert(error_.has_value());
      return std::forward<F>(f)(std::move(*error_));
    }
    assert(data_.has_value());
    assert(!error_.has_value());
    return decltype(f(std::declval<E>())){std::move(*data_)};
  }

private:
  std::optional<T> data_;
  std::optional<E> error_;
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
private:
  struct Void {
    bool value{true};
  };

public:
  Result() noexcept : data_{Void{}} {}
  Result(E&& error) noexcept : error_{std::move(error)} {}
  Result(Result&&) noexcept = default;
  ~Result() noexcept = default;
  auto operator=(Result&&) -> Result& = default;

  Result(const Result&) = delete;
  auto operator=(const Result&) -> Result& = delete;

  auto IsOk() const noexcept -> bool { return data_.has_value(); }

  auto IsErr() const noexcept -> bool { return error_.has_value(); }

  auto Ok() noexcept -> void {
    if (IsOk()) {
      assert(data_.has_value());
      assert(!error_.has_value());
      data_.reset();
      return;
    }
  }

  auto Err() noexcept -> std::optional<E> {
    if (IsErr()) {
      assert(!data_.has_value());
      assert(error_.has_value());
      auto error = std::move(error_);
      error_.reset();
      return error;
    }
    return std::nullopt;
  }

  template <typename F> auto AndThen(F&& f) noexcept -> decltype(f()) {
    static_assert(std::is_invocable_v<F>,
                  "Function must be invocable without arguments");
    if (IsOk()) {
      assert(data_.has_value());
      assert(!error_.has_value());
      data_.reset();
      return std::forward<F>(f)();
    }
    assert(!data_.has_value());
    assert(error_.has_value());
    return decltype(f()){std::move(*error_)};
  }

  template <typename F>
  auto OrElse(F&& f) noexcept -> decltype(f(std::declval<E>())) {
    static_assert(std::is_invocable_v<F, E>,
                  "Function must be invocable with E");
    if (IsErr()) {
      assert(!data_.has_value());
      assert(error_.has_value());
      return std::forward<F>(f)(std::move(*error_));
    }
    assert(data_.has_value());
    assert(!error_.has_value());
    data_.reset();
    return decltype(f(std::declval<E>())){};
  }

private:
  std::optional<Void> data_;
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
