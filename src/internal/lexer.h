#ifndef KERO_PEG_INTERNAL_LEXER_H
#define KERO_PEG_INTERNAL_LEXER_H

#include <functional>
#include <optional>
#include <string_view>

namespace kero {
namespace peg {

struct Span {
  size_t position;
  std::string_view value;

  Span(const size_t position, const std::string_view value) noexcept;
};

enum class TokenKind {
  kEndOfFile = 0,
  kWhitespace,
  kNewLine,
  kIdentifier,
  kLeftArrow,
  kExpression,
  kTerminal,
};

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

using Match =
    std::function<std::optional<std::string_view>(const LexerContext&)>;

struct Matcher {
  TokenKind kind;
  Match match;
  bool skip{false};
};

class Lexer {
public:
  Lexer(const std::string_view source) noexcept;
  Lexer(Lexer&&) noexcept = default;
  ~Lexer() = default;
  auto operator=(Lexer&&) noexcept -> Lexer& = default;

  Lexer(const Lexer&) = delete;
  auto operator=(const Lexer&) -> Lexer& = delete;

  auto Next() noexcept -> std::optional<Token>;

private:
  LexerContext context_;
  std::vector<Matcher> matchers_;
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_LEXER_H