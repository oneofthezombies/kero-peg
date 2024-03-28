#include "grammar_parser.h"

#include "grammar_context.h"
#include "lexer.h"
#include <iostream>

namespace kero {
namespace peg {

GrammarParser::GrammarParser(const GrammarContext& context) noexcept
    : context_(context) {}

auto GrammarParser::Parse() noexcept -> Result<void, Error> {
  Lexer lexer{context_.Source()};
  std::unordered_map<Identifier, Expression> rule_map;
  while (true) {
    auto next_res = lexer.Next();
    if (next_res.IsErr()) {
      const auto error = next_res.Err();
      std::cout << "error: " << *error << std::endl;
      break;
    }

    const auto token = next_res.Ok();
    std::cout << *token << std::endl;

    switch (token->kind) {
    case TokenKind::kEndOfFile:
      return {};
    default:
      break;
    }
  }

  return {};
}

auto operator<<(std::ostream& os, const GrammarParser::Error& error)
    -> std::ostream& {
  os << static_cast<int32_t>(error);
  return os;
}

} // namespace peg
} // namespace kero
