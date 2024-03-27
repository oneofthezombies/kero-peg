#include "lexer.h"
#include <cctype>

namespace kero {
namespace peg {

Span::Span(const size_t position, const std::string_view value) noexcept
    : position(position), value(value) {}

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
      Matcher{TokenKind::kEndOfFile,
              [](const LexerContext& ctx) -> std::optional<std::string_view> {
                if (const auto ch = ctx.Peek()) {
                  return std::nullopt;
                }

                return "";
              }},
      Matcher{TokenKind::kWhitespace,
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
      Matcher{TokenKind::kNewLine,
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
      Matcher{
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
      Matcher{TokenKind::kLeftArrow,
              [](const LexerContext& ctx) -> std::optional<std::string_view> {
                if (const auto ch = ctx.Peek()) {
                  if (const auto left_arrow = ctx.Match("<-")) {
                    return left_arrow;
                  }
                }

                return std::nullopt;
              }},
      Matcher{TokenKind::kExpression,
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
      Matcher{TokenKind::kTerminal,
              [](const LexerContext& ctx) -> std::optional<std::string_view> {
                const auto tokenize =
                    [](const LexerContext& ctx,
                       const char quote) -> std::optional<std::string_view> {
                  const auto source = ctx.Source();
                  for (size_t i = ctx.Position() + 1; i < source.size(); ++i) {
                    const auto current = source[i];
                    if (current == quote) {
                      return source.substr(ctx.Position(),
                                           i - ctx.Position() + 1);
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

auto Lexer::Next() noexcept -> std::optional<Token> {
  while (true) {
    std::optional<size_t> matcher_i = std::nullopt;
    std::optional<std::string_view> matched = std::nullopt;
    for (size_t i{}; i < matchers_.size(); ++i) {
      const auto& matcher = matchers_[i];
      if (const auto value = matcher.match(context_)) {
        matched = value;
        matcher_i = i;
        break;
      }
    }

    if (!matched) {
      // TODO: Error handling
      return std::nullopt;
    }

    context_.Consume(matched->size());

    if (!matcher_i) {
      // TODO: Error handling
      return std::nullopt;
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
