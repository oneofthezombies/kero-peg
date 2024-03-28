#include "lexer.h"

#include <cctype>
#include <ostream>

namespace kero {
namespace peg {

auto operator<<(std::ostream& os, const TokenKind kind) -> std::ostream& {
  switch (kind) {
  case TokenKind::kEndOfFile:
    os << "EndOfFile";
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
  }

  return os;
}

auto operator<<(std::ostream& os, const LexerNextError error) -> std::ostream& {
  switch (error) {
  case LexerNextError::kMatchFailed:
    os << "MatchFailed";
    break;
  case LexerNextError::kMatcherNotFound:
    os << "MatcherNotFound";
    break;
  }

  return os;
}

LexerContext::LexerContext(const std::string_view source) noexcept
    : source_(source) {}

auto LexerContext::Peek() const noexcept -> std::optional<char> {
  if (position_ >= source_.size()) {
    return std::nullopt;
  }

  return source_[position_];
}

auto LexerContext::Source() const noexcept -> std::string_view {
  return source_;
}

auto LexerContext::Position() const noexcept -> size_t { return position_; }

auto LexerContext::Line() const noexcept -> size_t { return line_; }

auto LexerContext::Column() const noexcept -> size_t { return column_; }

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
          const auto source = ctx.Source();
          for (size_t i = ctx.Position() + 1; i < source.size(); ++i) {
            const auto current = source[i];
            if (current == close) {
              return source.substr(ctx.Position(), i - ctx.Position() + 1);
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
          TokenKind::kEndOfFile,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            if (const auto ch = ctx.Peek()) {
              return std::nullopt;
            }

            return "";
          }},
      LexerMatcher{TokenKind::kWhitespace, OnMatchSimple({" ", "\t"}), true},
      LexerMatcher{TokenKind::kNewLine, OnMatchSimple({"\n", "\r\n"}), true},
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
          TokenKind::kQuotedTerminal,
          OnMatchAround({{'\'', '\''}, {'"', '"'}}),
      },
      LexerMatcher{
          TokenKind::kBracketedTerminal,
          OnMatchAround({{'[', ']'}}),
      },
      LexerMatcher{
          TokenKind::kNonTerminal,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            if (const auto ch = ctx.Peek()) {
              if (std::isalpha(*ch)) {
                const auto source = ctx.Source();
                for (size_t i = ctx.Position(); i < source.size(); ++i) {
                  const auto current = source[i];
                  if (!std::isalnum(current) && current != '_') {
                    return source.substr(ctx.Position(), i - ctx.Position());
                  }
                }

                return source.substr(ctx.Position());
              }
            }

            return std::nullopt;
          }},

      // NOTE: This is a comment for line alignment when using the
      // formatter.
  }; // matchers_
};

auto Lexer::Next() noexcept -> Result<Token, LexerNextError> {
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

    if (!matched) {
      return LexerNextError::kMatchFailed;
    }

    context_.Consume(matched->size());

    if (!matcher_i) {
      return LexerNextError::kMatcherNotFound;
    }

    const auto& matcher = matchers_[*matcher_i];
    if (matcher.skip) {
      continue;
    }

    return Token{matcher.kind, *matched};
  }
}

} // namespace peg
} // namespace kero
