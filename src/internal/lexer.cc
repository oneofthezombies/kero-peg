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
  case TokenKind::kIdentifier:
    os << "Identifier";
    break;
  case TokenKind::kLeftArrow:
    os << "LeftArrow";
    break;
  case TokenKind::kExpression:
    os << "Expression";
    break;
  case TokenKind::kTerminal:
    os << "Terminal";
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
      LexerMatcher{
          TokenKind::kWhitespace,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            if (const auto ch = ctx.Peek()) {
              if (const auto space = ctx.Match(" ")) {
                return space;
              } else if (const auto tab = ctx.Match("\t")) {
                return tab;
              }
            }

            return std::nullopt;
          },
          true},
      LexerMatcher{
          TokenKind::kNewLine,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            if (const auto ch = ctx.Peek()) {
              if (const auto unix_ln = ctx.Match("\n")) {
                return unix_ln;
              } else if (const auto windows_ln = ctx.Match("\r\n")) {
                return windows_ln;
              }
            }

            return std::nullopt;
          },
          true},
      LexerMatcher{
          TokenKind::kIdentifier,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            if (const auto ch = ctx.Peek()) {
              if (std::isalpha(*ch)) {
                const auto source = ctx.Source();
                for (size_t i = ctx.Position() + 1; i < source.size(); ++i) {
                  const auto current = source[i];
                  const auto valid = std::isalnum(current) || current == '_';
                  if (!valid) {
                    return source.substr(ctx.Position(), i - ctx.Position());
                  }
                }
              }
            }

            return std::nullopt;
          }},
      LexerMatcher{
          TokenKind::kLeftArrow,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            if (const auto ch = ctx.Peek()) {
              if (const auto left_arrow = ctx.Match("<-")) {
                return left_arrow;
              }
            }

            return std::nullopt;
          }},
      LexerMatcher{
          TokenKind::kExpression,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            if (const auto ch = ctx.Peek()) {
              const auto source = ctx.Source();
              for (size_t i = ctx.Position(); i < source.size(); ++i) {
                const auto current = source[i];
                if (current == '\n' || current == '\r') {
                  return source.substr(ctx.Position(), i - ctx.Position());
                }
              }
            }

            return std::nullopt;
          }},
      LexerMatcher{
          TokenKind::kTerminal,
          [](const LexerContext& ctx) -> std::optional<std::string_view> {
            const auto tokenize =
                [](const LexerContext& ctx,
                   const char quote) -> std::optional<std::string_view> {
              const auto source = ctx.Source();
              for (size_t i = ctx.Position() + 1; i < source.size(); ++i) {
                const auto current = source[i];
                if (current == quote) {
                  return source.substr(ctx.Position(), i - ctx.Position() + 1);
                }
              }

              return std::nullopt;
            };

            if (const auto ch = ctx.Peek()) {
              if (*ch == '"') {
                return tokenize(ctx, '"');
              } else if (*ch == '\'') {
                return tokenize(ctx, '\'');
              }
            }

            return std::nullopt;
          }}};
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
