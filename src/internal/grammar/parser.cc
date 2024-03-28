#include "parser.h"

#include <iostream>

namespace kero {
namespace peg {
namespace grammar {

Parser::Parser(const std::string_view source) noexcept : lexer_{source} {}

auto Parser::Parse() noexcept -> void {
  auto next_res = lexer_.Next();
  if (next_res.IsErr()) {
    std::cout << *next_res.Err() << std::endl;
    return;
  }

  const auto token{std::move(*next_res.Ok())};
  switch (token.kind) {
  case TokenKind::kNonTerminal:
    auto inner_token = lexer_.Next();
    if (inner_token.IsErr()) {
      std::cout << *inner_token.Err() << std::endl;
      return;
    }
    break;
  }
}

} // namespace grammar
} // namespace peg
} // namespace kero
