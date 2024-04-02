#include "parser.h"

#include <iostream>

namespace kero {
namespace peg {
namespace grammar {

Parser::Parser(const std::string_view source) noexcept : lexer_{source} {}

auto Parser::Parse() noexcept -> void {
  auto stack = std::vector<Node*>{};
  stack.push_back(new RuleSetNode{});

  while (true) {
    auto res = lexer_.Next();
    if (res.IsErr()) {
      std::cerr << "Error: " << *res.Err() << std::endl;
      break;
    }

    auto token = *res.Ok();
    if (token.kind == TokenKind::kEndOfInput) {
      break;
    }

    switch (token.kind) {}
  }
}

} // namespace grammar
} // namespace peg
} // namespace kero
