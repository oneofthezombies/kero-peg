#ifndef KERO_PEG_INTERNAL_LEXER_H
#define KERO_PEG_INTERNAL_LEXER_H

#include <functional>
#include <optional>
#include <string_view>

#include "core.h"

namespace kero {
namespace peg {

enum class TokenKind {
  kEndOfFile = 0,
  kWhitespace,
  kNewLine,
  kIdentifier,
  kLeftArrow,
  kExpression,
  kTerminal,
};

auto operator<<(std::ostream& os, const TokenKind kind) -> std::ostream&;

struct Token {
  TokenKind kind;
  std::string_view value;
};

class LexerContext {
public:
  LexerContext(const std::string_view source) noexcept;

  auto Peek() const noexcept -> std::optional<char>;
  auto Source() const noexcept -> std::string_view;
  auto Position() const noexcept -> size_t;
  auto Line() const noexcept -> size_t;
  auto Column() const noexcept -> size_t;
  auto Match(const std::string_view expected) const noexcept
      -> std::optional<std::string_view>;
  auto Consume(const size_t size) noexcept -> bool;

private:
  std::string_view source_;
  size_t position_{};
  size_t line_{1};
  size_t column_{1};
};

using OnMatch =
    std::function<std::optional<std::string_view>(const LexerContext&)>;

struct LexerMatcher {
  TokenKind kind;
  OnMatch on_match;
  bool skip{false};
};

enum class LexerNextError : int32_t {
  kMatchFailed = 0,
  kMatcherNotFound,
};

auto operator<<(std::ostream& os, const LexerNextError error) -> std::ostream&;

class Lexer {
public:
  Lexer(const std::string_view source) noexcept;
  Lexer(Lexer&&) noexcept = default;
  ~Lexer() = default;
  auto operator=(Lexer&&) noexcept -> Lexer& = default;

  Lexer(const Lexer&) = delete;
  auto operator=(const Lexer&) -> Lexer& = delete;

  auto Next() noexcept -> Result<Token, LexerNextError>;

private:
  LexerContext context_;
  std::vector<LexerMatcher> matchers_;
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_LEXER_H