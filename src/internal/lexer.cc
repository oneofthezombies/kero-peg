#include "lexer.h"

namespace kero {
namespace peg {

Eaten::Eaten(const char character, const size_t position) noexcept
    : character(character), position(position) {}

Lexer::Lexer(const std::string_view source) noexcept : source_(source) {}

auto Lexer::Next() noexcept -> std::optional<std::string_view> {
  while (true) {
    if (const auto eaten = Eat()) {
      if (IsWhitespace(eaten->character)) {
        continue;
      } else if (eaten->character == '"') {
        return ParseQuoted('"', eaten->position);
      } else if (eaten->character == '\'') {
        return ParseQuoted('\'', eaten->position);
      } else {
        return ParseNonquoted(eaten->character, eaten->position);
      }
    } else {
      return std::nullopt;
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

auto Lexer::ParseQuoted(const char quote_like,
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

auto Lexer::ParseNonquoted(const char character,
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

auto Lexer::IsWhitespace(const char character) const noexcept -> bool {
  return character == ' ' || character == '\t' || character == '\n' ||
         character == '\r';
}

} // namespace peg
} // namespace kero
