#ifndef KERO_PEG_CORE_H
#define KERO_PEG_CORE_H

#include <cstdint>
#include <optional>
#include <variant>

namespace kero {
namespace peg {

enum class ErrorCode : int32_t {
  kUnknown = 0,
  kEndOfGrammarSource,
  kTokenNotFound,
};

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

  auto IsError() const noexcept -> bool {
    return std::holds_alternative<E>(var_);
  }

  auto Ok() noexcept -> std::optional<T> {
    if (IsOk()) {
      return std::get<T>(var_);
    }

    return std::nullopt;
  }

  auto Error() noexcept -> std::optional<E> {
    if (IsError()) {
      return std::get<E>(var_);
    }

    return std::nullopt;
  }

private:
  std::variant<T, E> var_;
};

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

  auto IsError() const noexcept -> bool { return !!error_; }

  auto Ok() noexcept -> void {}

  auto Error() noexcept -> std::optional<E> {
    if (IsError()) {
      return error_;
    }

    return std::nullopt;
  }

private:
  std::optional<E> error_;
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_CORE_H
