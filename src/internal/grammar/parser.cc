#include "parser.h"

namespace kero {
namespace peg {
namespace grammar {

Parser::Parser(const std::string_view source) noexcept : lexer_{source} {}

auto Parser::Parse() noexcept -> std::optional<Node> {
  auto node{std::optional<Node>{}};
  while (true) {
    auto next_res = lexer_.Next();
    if (next_res.IsErr()) {
      break;
    }

    const auto token{std::move(*next_res.Ok())};
    if (token.kind == TokenKind::kEndOfFile) {
      break;
    }

    switch (token.kind) {
    case TokenKind::kNonTerminal:
      node = Node{token, NodeKind::kNonTerminal};
      break;
    case TokenKind::kQuotedTerminal:
    case TokenKind::kBracketedTerminal:
      node = Node{token, NodeKind::kTerminal};
      break;
    case TokenKind::kLeftParenthesis: {
      auto inner_node = Parse();
      if (!inner_node.has_value()) {
        return std::nullopt;
      }
      node = std::move(*inner_node);
      break;
    }
    case TokenKind::kRightParenthesis:
      return node;
    default:
      break;
    }
  }

  return node;
}

} // namespace grammar
} // namespace peg
} // namespace kero
