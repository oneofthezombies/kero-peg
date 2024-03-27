#ifndef KERO_PEG_INTERNAL_LEXER_H
#define KERO_PEG_INTERNAL_LEXER_H

#include <string_view>

namespace kero {
namespace peg {

struct Eaten {
  char character{};
  size_t position{};
};

class Lexer {
public:
  Lexer(const std::string_view source) noexcept;
  Lexer(Lexer&&) noexcept = default;
  ~Lexer() = default;
  auto operator=(Lexer&&) noexcept -> Lexer& = default;

  Lexer(const Lexer&) = delete;
  auto operator=(const Lexer&) -> Lexer& = delete;

  auto Next() noexcept -> std::optional<std::string_view>;
  auto Source() const noexcept -> std::string_view;
  auto Position() const noexcept -> size_t;

private:
  auto Eat() noexcept -> std::optional<Eaten>;
  auto ParseTerminal(const char quote_like, const size_t quote_start) noexcept
      -> std::optional<std::string_view>;
  auto ParseNonTerminal(const char character,
                        const size_t character_start) noexcept
      -> std::optional<std::string_view>;

  std::string_view source_;
  size_t position_{};
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_LEXER_H