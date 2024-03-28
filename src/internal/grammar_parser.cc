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
    if (next_res.IsError()) {
      const auto error = next_res.Error();
      std::cout << "error: " << *error << std::endl;
      break;
    }

    const auto token = next_res.Ok();
    const auto kind = token->kind;
    const auto value = token->value;
    std::cout << "kind: " << kind << " value: " << value << std::endl;

    switch (kind) {
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
