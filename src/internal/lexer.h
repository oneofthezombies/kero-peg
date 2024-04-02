#ifndef KERO_PEG_INTERNAL_GRAMMAR_LEXER_H
#define KERO_PEG_INTERNAL_GRAMMAR_LEXER_H

#include <functional>
#include <optional>
#include <string_view>

#include "./core.h"

namespace kero {
namespace peg {

enum class TokenKind : int32_t {
  kEndOfInput = 0,
  kWhitespace,        // ' ' or \t
  kNewLine,           // \n or \r\n
  kLeftArrow,         // <-
  kLeftParenthesis,   // (
  kRightParenthesis,  // )
  kAsterisk,          // *
  kPlus,              // +
  kQuestionMark,      // ?
  kAmpersand,         // &
  kExclamationMark,   // !
  kSlash,             // /
  kDot,               // .
  kCaret,             // ^
  kNonTerminal,       // e.g. A
  kQuotedTerminal,    // e.g. 'a' or "a"
  kBracketedTerminal, // e.g. [a]
};

auto operator<<(std::ostream& os, const TokenKind kind) noexcept
    -> std::ostream&;

struct Location {
  size_t position;
  size_t line;
  size_t column;

  Location(const size_t position, const size_t line,
           const size_t column) noexcept;
};

auto operator<<(std::ostream& os, const Location& location) noexcept
    -> std::ostream&;

struct Token {
  Location original_start;
  Location original_end;
  std::string_view original;
  std::string_view value;
  TokenKind kind;
};

auto operator<<(std::ostream& os, const Token& token) noexcept -> std::ostream&;

class LexerContext {
public:
  LexerContext(const std::string_view source) noexcept;

  auto Peek() const noexcept -> std::optional<char>;
  auto GetSource() const noexcept -> std::string_view;
  auto GetPosition() const noexcept -> size_t;
  auto GetLine() const noexcept -> size_t;
  auto GetColumn() const noexcept -> size_t;
  auto GetLocation() const noexcept -> Location;
  auto Match(const std::string_view expected) const noexcept
      -> std::optional<std::string_view>;
  auto Consume(const size_t size) noexcept -> bool;

private:
  std::string_view source_;
  size_t position_{};
  size_t line_{1};
  size_t column_{1};
};

enum class TokenizeErrorCode : int32_t {
  kMatchFailed = 0,
  kInternalMatcherNotFound,
  kBracketedTerminalEmpty,
  kBracketedTerminalMinusSignLeftNotFound,
  kBracketedTerminalMinusSignRightNotFound,
  kBracketedTerminalMinusSignRightNotNumber,
  kBracketedTerminalMinusSignRightNumberEqualWithLeft,
  kBracketedTerminalMinusSignRightNumberLessThanLeft,
  kBracketedTerminalMinusSignRightNotLowercase,
  kBracketedTerminalMinusSignRightLowercaseEqualWithLeft,
  kBracketedTerminalMinusSignRightLowercaseLessThanLeft,
  kBracketedTerminalMinusSignRightNotUppercase,
  kBracketedTerminalMinusSignRightUppercaseEqualWithLeft,
  kBracketedTerminalMinusSignRightUppercaseLessThanLeft,
};

auto operator<<(std::ostream& os, const TokenizeErrorCode error) noexcept
    -> std::ostream&;

struct TokenizeError {
  TokenizeErrorCode code;
  Location location;

  TokenizeError(const TokenizeErrorCode code, const Location location) noexcept;
};

auto operator<<(std::ostream& os, const TokenizeError& error) noexcept
    -> std::ostream&;

using OnMatch =
    std::function<std::optional<std::string_view>(const LexerContext&)>;
using OnParse = std::function<Result<std::string_view, TokenizeError>(
    const LexerContext&, const std::string_view)>;

struct LexerMatcher {
  TokenKind kind;
  OnMatch on_match;
  std::optional<OnParse> on_parse{std::nullopt};
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

  auto Next() noexcept -> Result<Token, TokenizeError>;

private:
  LexerContext context_;
  std::vector<LexerMatcher> matchers_;
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_GRAMMAR_LEXER_H
