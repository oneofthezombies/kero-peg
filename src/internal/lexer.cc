#include "lexer.h"

namespace kero {
namespace peg {

auto IsWhitespace(const char c) noexcept -> bool {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

Lexer::Lexer(const std::string_view source) noexcept : source_(source) {}

auto Lexer::Next() noexcept -> std::optional<std::string_view> {
  while (true) {
    const auto eaten_opt = Eat();
    if (!eaten_opt) {
      return std::nullopt;
    }

    const auto eaten = *eaten_opt;
    if (IsWhitespace(eaten.character)) {
      continue;
    } else if (eaten.character == '"') {
      return ParseTerminal('"', eaten.position);
    } else if (eaten.character == '\'') {
      return ParseTerminal('\'', eaten.position);
    } else {
      return ParseNonTerminal(eaten.character, eaten.position);
    }
  }
}

auto Lexer::Eat() noexcept -> std::optional<Eaten> {
  if (position_ >= source_.size()) {
    return std::nullopt;
  }

  const Eaten eaten{source_[position_], position_};
  ++position_;
  return eaten;
}

auto Lexer::ParseTerminal(const char quote_like,
                          const size_t quote_start) noexcept
    -> std::optional<std::string_view> {
  while (true) {
    if (const auto eaten = Eat()) {
      if (eaten->character == quote_like) {
        const auto character_start = quote_start + 1;
        const auto character_end = eaten->position;
        const auto character_size = character_end - character_start;
        return source_.substr(character_start, character_size);
      }
    } else {
      return std::nullopt;
    }
  }
}

auto Lexer::ParseNonTerminal(const char character,
                             const size_t character_start) noexcept
    -> std::optional<std::string_view> {
  while (true) {
    if (const auto eaten = Eat()) {
      if (IsWhitespace(eaten->character)) {
        const auto character_end = eaten->position;
        const auto character_size = character_end - character_start;
        return source_.substr(character_start, character_size);
      }
    } else {
      return source_.substr(character_start);
    }
  }
}

auto Lexer::Source() const noexcept -> std::string_view { return source_; }

auto Lexer::Position() const noexcept -> size_t { return position_; }

} // namespace peg
} // namespace kero
