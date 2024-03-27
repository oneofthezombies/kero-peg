#include "grammar_parser.h"

#include "grammar_context.h"
#include "lexer.h"
#include <iostream>

namespace kero {
namespace peg {

GrammarParser::GrammarParser(const GrammarContext& context) noexcept
    : context_(context) {}

auto GrammarParser::Parse() noexcept -> void {
  Lexer lexer{context_.Source()};
  while (auto token = lexer.Next()) {
    std::cout << *token << std::endl;
  }
}

} // namespace peg
} // namespace kero
