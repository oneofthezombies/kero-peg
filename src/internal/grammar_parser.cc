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
    const auto identifier = lexer.Next();
    if (!identifier) {
      return Error::kIdentifierNotFound;
    }

    if (rule_map.find(*identifier) != rule_map.end()) {
      return Error::kIdentifierDuplicate;
    }

    const auto left_arrow = lexer.Next();
    if (!left_arrow) {
      return Error::kLeftArrowNotFound;
    }

    if (left_arrow != "<-") {
      return Error::kLeftArrowInvalid;
    }

    const auto expression = lexer.Next();
    if (!expression) {
      return Error::kExpressionNotFound;
    }

    rule_map.emplace(*identifier, *expression);
  }

  for (const auto& [identifier, expression] : rule_map) {
    std::cout << "Rule: " << identifier << " <- " << expression << std::endl;
  }
}

} // namespace peg
} // namespace kero
