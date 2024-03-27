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
    const auto token = lexer.Next();
    if (!token) {
      std::cout << "token is empty" << std::endl;
      break;
    }

    const auto kind = token->kind;
    const auto value = token->value;
    std::cout << "kind: " << static_cast<int>(kind) << " value: " << value
              << std::endl;

    switch (kind) {
    case TokenKind::kEndOfFile:
      return {};
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
