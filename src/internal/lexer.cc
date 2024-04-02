#include "./lexer.h"

#include <cctype>
#include <ostream>

#include "./core.h"

namespace kero {
namespace peg {

auto operator<<(std::ostream& os, const TokenKind kind) noexcept
    -> std::ostream& {
  switch (kind) {
  case TokenKind::kEndOfInput:
    os << "EndOfInput";
    break;
  case TokenKind::kWhitespace:
    os << "Whitespace";
    break;
  case TokenKind::kNewLine:
    os << "NewLine";
    break;
  case TokenKind::kLeftArrow:
    os << "LeftArrow";
    break;
  case TokenKind::kQuotedTerminal:
    os << "QuotedTerminal";
    break;
  case TokenKind::kNonTerminal:
    os << "NonTerminal";
    break;
  case TokenKind::kLeftParenthesis:
    os << "LeftParenthesis";
    break;
  case TokenKind::kRightParenthesis:
    os << "RightParenthesis";
    break;
  case TokenKind::kAsterisk:
    os << "Asterisk";
    break;
  case TokenKind::kPlus:
    os << "Plus";
    break;
  case TokenKind::kQuestionMark:
    os << "QuestionMark";
    break;
  case TokenKind::kAmpersand:
    os << "Ampersand";
    break;
  case TokenKind::kExclamationMark:
    os << "ExclamationMark";
    break;
  case TokenKind::kSlash:
    os << "Slash";
    break;
  case TokenKind::kDot:
    os << "Dot";
    break;
  case TokenKind::kBracketedTerminal:
    os << "BracketedTerminal";
    break;
  case TokenKind::kCaret:
    os << "Caret";
    break;
  }

  return os;
}

Location::Location(const size_t position, const size_t line,
                   const size_t column) noexcept
    : position{position}, line{line}, column{column} {}

auto operator<<(std::ostream& os, const Location& location) noexcept
    -> std::ostream& {
  os << "Location{";
  os << "position=" << location.position;
  os << ", ";
  os << "line=" << location.line;
  os << ", ";
  os << "column=" << location.column;
  os << "}";
  return os;
}

auto operator<<(std::ostream& os, const Token& token) noexcept
    -> std::ostream& {
  os << "Token{";
  os << "kind=" << token.kind;
  os << ", ";
  os << "value=" << token.value;
  os << ", ";
  os << "original=" << token.original;
  os << ", ";
  os << "original_start=" << token.original_start;
  os << ", ";
  os << "original_end=" << token.original_end;
  os << "}";
  return os;
}

auto operator<<(std::ostream& os, const TokenizeErrorCode error) noexcept
    -> std::ostream& {
  switch (error) {
  case TokenizeErrorCode::kMatchFailed:
    os << "MatchFailed";
    break;
  case TokenizeErrorCode::kInternalMatcherNotFound:
    os << "InternalMatcherNotFound";
    break;
  case TokenizeErrorCode::kBracketedTerminalEmpty:
    os << "BracketedTerminalEmpty";
    break;
  case TokenizeErrorCode::kBracketedTerminalMinusSignLeftNotFound:
    os << "BracketedTerminalMinusSignLeftNotFound";
    break;
  case TokenizeErrorCode::kBracketedTerminalMinusSignRightNotFound:
    os << "BracketedTerminalMinusSignRightNotFound";
    break;
  case TokenizeErrorCode::kBracketedTerminalMinusSignRightNotNumber:
    os << "BracketedTerminalMinusSignRightNotNumber";
    break;
  case TokenizeErrorCode::kBracketedTerminalMinusSignRightNumberEqualWithLeft:
    os << "BracketedTerminalMinusSignRightNumberEqualWithLeft";
    break;
  case TokenizeErrorCode::kBracketedTerminalMinusSignRightNumberLessThanLeft:
    os << "BracketedTerminalMinusSignRightNumberLessThanLeft";
    break;
  case TokenizeErrorCode::kBracketedTerminalMinusSignRightNotLowercase:
    os << "BracketedTerminalMinusSignRightNotLowercase";
    break;
  case TokenizeErrorCode::
      kBracketedTerminalMinusSignRightLowercaseEqualWithLeft:
    os << "BracketedTerminalMinusSignRightLowercaseEqualWithLeft";
    break;
  case TokenizeErrorCode::kBracketedTerminalMinusSignRightLowercaseLessThanLeft:
    os << "BracketedTerminalMinusSignRightLowercaseLessThanLeft";
    break;
  case TokenizeErrorCode::kBracketedTerminalMinusSignRightNotUppercase:
    os << "BracketedTerminalMinusSignRightNotUppercase";
    break;
  case TokenizeErrorCode::
      kBracketedTerminalMinusSignRightUppercaseEqualWithLeft:
    os << "BracketedTerminalMinusSignRightUppercaseEqualWithLeft";
    break;
  case TokenizeErrorCode::kBracketedTerminalMinusSignRightUppercaseLessThanLeft:
    os << "BracketedTerminalMinusSignRightUppercaseLessThanLeft";
    break;
  }

  return os;
}

TokenizeError::TokenizeError(const TokenizeErrorCode code,
                             const Location location) noexcept
    : code{code}, location{location} {}

auto operator<<(std::ostream& os, const TokenizeError& error) noexcept
    -> std::ostream& {
  os << "TokenizeError{";
  os << "code=" << error.code;
  os << ", ";
  os << "location=" << error.location;
  os << "}";
  return os;
}

LexerContext::LexerContext(const std::string_view source) noexcept
    : source_{source} {}

auto LexerContext::Peek() const noexcept -> std::optional<char> {
  if (position_ >= source_.size()) {
    return std::nullopt;
  }

  return source_[position_];
}

auto LexerContext::GetSource() const noexcept -> std::string_view {
  return source_;
}

auto LexerContext::GetPosition() const noexcept -> size_t { return position_; }

auto LexerContext::GetLine() const noexcept -> size_t { return line_; }

auto LexerContext::GetColumn() const noexcept -> size_t { return column_; }

auto LexerContext::GetLocation() const noexcept -> Location {
  return Location{position_, line_, column_};
}

auto LexerContext::Match(const std::string_view expected) const noexcept
    -> std::optional<std::string_view> {
  if (position_ + expected.size() > source_.size()) {
    return std::nullopt;
  }

  for (size_t i = 0; i < expected.size(); ++i) {
    if (source_[position_ + i] != expected[i]) {
      return std::nullopt;
    }
  }

  return source_.substr(position_, expected.size());
}

auto LexerContext::Consume(const size_t size) noexcept -> bool {
  if (position_ + size > source_.size()) {
    return false;
  }

  for (size_t i = 0; i < size; ++i) {
    const auto current = source_[position_ + i];
    if (current == '\r') {
      continue;
    }

    if (current == '\n') {
      ++line_;
      column_ = 1;
    } else {
      ++column_;
    }
  }

  position_ += size;
  return true;
}

auto OnMatchSimple(std::vector<std::string_view>&& matches) -> OnMatch {
  return [matches = std::move(matches)](
             const LexerContext& ctx) -> std::optional<std::string_view> {
    if (const auto ch = ctx.Peek()) {
      for (const auto& match : matches) {
        if (const auto value = ctx.Match(match)) {
          return value;
        }
      }
    }

    return std::nullopt;
  };
}

auto OnMatchAround(std::vector<std::pair<char, char>>&& matches) -> OnMatch {
  return [matches = std::move(matches)](
             const LexerContext& ctx) -> std::optional<std::string_view> {
    if (const auto ch = ctx.Peek()) {
      for (const auto& [open, close] : matches) {
        if (*ch == open) {
          const auto source = ctx.GetSource();
          for (size_t i = ctx.GetPosition() + 1; i < source.size(); ++i) {
            const auto current = source[i];
            if (current == close) {
              return source.substr(ctx.GetPosition(),
                                   i - ctx.GetPosition() + 1);
            }
          }

          return std::nullopt;
        }
      }
    }

    return std::nullopt;
  };
}

Lexer::Lexer(const std::string_view source) noexcept : context_{source} {
  matchers_ = {
      LexerMatcher{
          TokenKind::kEndOfInput,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            if (const auto ch = ctx.Peek()) {
              return std::nullopt;
            }

            return "";
          }},
      LexerMatcher{TokenKind::kWhitespace, OnMatchSimple({" ", "\t"}),
                   std::nullopt, true},
      LexerMatcher{TokenKind::kNewLine, OnMatchSimple({"\n", "\r\n"}),
                   std::nullopt},
      LexerMatcher{
          TokenKind::kLeftArrow,
          OnMatchSimple({"<-"}),
      },
      LexerMatcher{
          TokenKind::kLeftParenthesis,
          OnMatchSimple({"("}),
      },
      LexerMatcher{
          TokenKind::kRightParenthesis,
          OnMatchSimple({")"}),
      },
      LexerMatcher{
          TokenKind::kAsterisk,
          OnMatchSimple({"*"}),
      },
      LexerMatcher{
          TokenKind::kPlus,
          OnMatchSimple({"+"}),
      },
      LexerMatcher{
          TokenKind::kQuestionMark,
          OnMatchSimple({"?"}),
      },
      LexerMatcher{
          TokenKind::kAmpersand,
          OnMatchSimple({"&"}),
      },
      LexerMatcher{
          TokenKind::kExclamationMark,
          OnMatchSimple({"!"}),
      },
      LexerMatcher{
          TokenKind::kSlash,
          OnMatchSimple({"/"}),
      },
      LexerMatcher{
          TokenKind::kDot,
          OnMatchSimple({"."}),
      },
      LexerMatcher{
          TokenKind::kCaret,
          OnMatchSimple({"^"}),
      },
      LexerMatcher{TokenKind::kQuotedTerminal,
                   OnMatchAround({{'\'', '\''}, {'"', '"'}}),
                   [](const LexerContext& ctx, const std::string_view original)
                       -> Result<std::string_view, TokenizeError> {
                     return original.substr(1, original.size() - 2);
                   }},
      LexerMatcher{
          TokenKind::kBracketedTerminal, OnMatchAround({{'[', ']'}}),
          [](const LexerContext& ctx, const std::string_view original)
              -> Result<std::string_view, TokenizeError> {
            std::optional<char> left{};
            std::optional<char> right{};
            auto value{original.substr(1, original.size() - 2)};
            auto location = ctx.GetLocation();
            location.position += 1;
            location.column += 1;

            if (value.empty()) {
              return TokenizeError{TokenizeErrorCode::kBracketedTerminalEmpty,
                                   location};
            }

            for (size_t i = 0; i < value.size(); ++i) {
              auto error = [&location, i](const TokenizeErrorCode code)
                  -> Result<std::string_view, TokenizeError> {
                location.position += i;
                location.column += i;
                return TokenizeError{code, location};
              };

              const auto current = value[i];
              if (i > 0) {
                left = value[i - 1];
              } else {
                left = std::nullopt;
              }

              if (i < value.size() - 1) {
                right = value[i + 1];
              } else {
                right = std::nullopt;
              }

              if (current == '-') {
                if (!left) {
                  return error(TokenizeErrorCode::
                                   kBracketedTerminalMinusSignLeftNotFound);
                }

                if (!right) {
                  return error(TokenizeErrorCode::
                                   kBracketedTerminalMinusSignRightNotFound);
                }

                if (std::isdigit(*left)) {
                  if (!std::isdigit(*right)) {
                    return error(TokenizeErrorCode::
                                     kBracketedTerminalMinusSignRightNotNumber);
                  }

                  if (*left == *right) {
                    return error(
                        TokenizeErrorCode::
                            kBracketedTerminalMinusSignRightNumberEqualWithLeft);
                  }

                  if (*left > *right) {
                    return error(
                        TokenizeErrorCode::
                            kBracketedTerminalMinusSignRightNumberLessThanLeft);
                  }
                } else if (std::islower(*left)) {
                  if (!std::islower(*right)) {
                    return error(
                        TokenizeErrorCode::
                            kBracketedTerminalMinusSignRightNotLowercase);
                  }

                  if (*left == *right) {
                    return error(
                        TokenizeErrorCode::
                            kBracketedTerminalMinusSignRightLowercaseEqualWithLeft);
                  }

                  if (*left > *right) {
                    return error(
                        TokenizeErrorCode::
                            kBracketedTerminalMinusSignRightLowercaseLessThanLeft);
                  }
                } else if (std::isupper(*left)) {
                  if (!std::isupper(*right)) {
                    return error(
                        TokenizeErrorCode::
                            kBracketedTerminalMinusSignRightNotUppercase);
                  }

                  if (*left == *right) {
                    return error(
                        TokenizeErrorCode::
                            kBracketedTerminalMinusSignRightUppercaseEqualWithLeft);
                  }

                  if (*left > *right) {
                    return error(
                        TokenizeErrorCode::
                            kBracketedTerminalMinusSignRightUppercaseLessThanLeft);
                  }
                }
              }
            }

            return value;
          }},
      LexerMatcher{
          TokenKind::kNonTerminal,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            if (const auto ch = ctx.Peek()) {
              if (std::isalpha(*ch)) {
                const auto source = ctx.GetSource();
                for (size_t i = ctx.GetPosition(); i < source.size(); ++i) {
                  const auto current = source[i];
                  if (!std::isalnum(current) && current != '_') {
                    return source.substr(ctx.GetPosition(),
                                         i - ctx.GetPosition());
                  }
                }

                return source.substr(ctx.GetPosition());
              }
            }

            return std::nullopt;
          }},

      // NOTE: This is a comment for line alignment when using the
      // formatter.
  }; // matchers_
};

auto Lexer::Next() noexcept -> Result<Token, TokenizeError> {
  while (true) {
    std::optional<size_t> matcher_i = std::nullopt;
    std::optional<std::string_view> matched = std::nullopt;
    for (size_t i{}; i < matchers_.size(); ++i) {
      const auto& matcher = matchers_[i];
      if (const auto value = matcher.on_match(context_)) {
        matched = value;
        matcher_i = i;
        break;
      }
    }

    const auto original_start = context_.GetLocation();
    if (!matched) {
      return TokenizeError{TokenizeErrorCode::kMatchFailed, original_start};
    }

    context_.Consume(matched->size());
    const auto original_end = context_.GetLocation();

    if (!matcher_i) {
      return TokenizeError{TokenizeErrorCode::kInternalMatcherNotFound,
                           original_start};
    }

    const auto& matcher = matchers_[*matcher_i];
    if (matcher.skip) {
      continue;
    }

    const auto original = *matched;
    if (matcher.on_parse) {
      auto res = (*matcher.on_parse)(context_, original);
      if (res.IsErr()) {
        return *res.Err();
      }

      return Token{
          original_start, original_end, original, *res.Ok(), matcher.kind,
      };
    }

    return Token{original_start, original_end, original, original,
                 matcher.kind};
  }
}

} // namespace peg
} // namespace kero
