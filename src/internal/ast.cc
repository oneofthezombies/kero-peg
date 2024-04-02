#include "ast.h"

namespace kero {
namespace peg {
namespace ast {

auto IsExpressionNodeKind(const NodeKind kind) noexcept -> bool {
  switch (kind) {
  case NodeKind::kSequence:
  case NodeKind::kOrderedChoice:
  case NodeKind::kZeroOrMore:
  case NodeKind::kOneOrMore:
  case NodeKind::kOptional:
  case NodeKind::kAndPredicate:
  case NodeKind::kNotPredicate:
  case NodeKind::kGroup:
  case NodeKind::kAnyCharacter:
  case NodeKind::kNonTerminal:
  case NodeKind::kQuotedTerminal:
  case NodeKind::kBracketedTerminal:
    return true;
  case NodeKind::kRuleSet:
  case NodeKind::kRule:
    return false;
  }
}

Parser::Parser(Lexer&& lexer) noexcept : lexer_{std::move(lexer)} {}

auto Parser::NextToken() noexcept -> Result<void, Error> {
  if (auto res = lexer_.Next(); res.IsErr()) {
    return Result<void, Error>{Error{*res.Err()}};
  } else {
    lookahead_ = *res.Ok();
    return Result<void, Error>{};
  }
}

auto Parser::ParseRuleSet() noexcept -> Result<NodePtr, Error> {
  auto rule_set{std::make_unique<RuleSet>()};
  while (true) {
    auto res = ParseRule();
    if (res.IsErr()) {
      return res;
    }
    rule_set->AddRule(*res.Ok());
  }
  return Result<NodePtr, Error>{std::move(rule_set)};
}

auto Parser::ParseRule() noexcept -> Result<NodePtr, Error> {
  if (auto res = NextToken(); res.IsErr()) {
    return Result<NodePtr, Error>{std::move(*res.Err())};
  }
  if (auto res = ParseNonTerminal(); res.IsErr()) {
    return res;
  } else {
    auto non_terminal = *res.Ok();
    if (auto res = ConsumeLeftArrowToken(); res.IsErr()) {
      return Result<NodePtr, Error>{std::move(*res.Err())};
    }
    if (auto res = ParseExpression(); res.IsErr()) {
      return Result<NodePtr, Error>{std::move(*res.Err())};
    }
    return Result<NodePtr, Error>{
        std::make_unique<Rule>(std::move(non_terminal), std::move(*res.Ok()))};
  }
}

auto Parser::ParseNonTerminal() noexcept -> Result<NodePtr, Error> {
  if (auto res = lexer_.Next(); res.IsErr()) {
    return Result<NodePtr, Error>{Error{*res.Err()}};
  } else {
    auto token = *res.Ok();
    if (token.kind != TokenKind::kNonTerminal) {
      return Result<NodePtr, Error>{
          Error{std::move(token), ErrorCode::kTokenNotNonTerminal}};
    }
    return Result<NodePtr, Error>{
        std::make_unique<NonTerminal>(std::move(token))};
  }
}

auto Parser::ConsumeLeftArrowToken() noexcept -> Result<void, Error> {
  if (auto res = lexer_.Next(); res.IsErr()) {
    return Result<void, Error>{Error{*res.Err()}};
  } else {
    auto left_arrow = *res.Ok();
    if (left_arrow.kind != TokenKind::kLeftArrow) {
      return Result<void, Error>{
          Error{std::move(left_arrow), ErrorCode::kTokenNotLeftArrow}};
    }
    return Result<void, Error>{};
  }
}

auto Parser::ParseExpression() noexcept -> Result<NodePtr, Error> {
  if (auto res = lexer_.Next(); res.IsErr()) {
    return Result<NodePtr, Error>{Error{*res.Err()}};
  } else {
    auto token = *res.Ok();
    switch (token.kind) {
    case TokenKind::kLeftParenthesis:
      return ParseGroup();
    case TokenKind::kAmpersand:
      return ParseAndPredicate();
    case TokenKind::kExclamationMark:
      return ParseNotPredicate();
    case TokenKind::kDot:
      return ParseAnyCharacter();
    case TokenKind::kNonTerminal:
      return ParseNonTerminal();
    case TokenKind::kQuotedTerminal:
      return ParseQuotedTerminal();
    case TokenKind::kBracketedTerminal:
      return ParseBracketedTerminal();
    }
  }
}

auto Parser::ParseGroup() noexcept -> Result<NodePtr, Error> {
  if (auto expr_res = ParseExpression(); expr_res.IsErr()) {
    return expr_res;
  } else {
    if (auto right_paren_res = lexer_.Next(); right_paren_res.IsErr()) {
      return Result<NodePtr, Error>{Error{*right_paren_res.Err()}};
    } else {
      auto right_paren = *right_paren_res.Ok();
      if (right_paren.kind != TokenKind::kRightParenthesis) {
        return Result<NodePtr, Error>{Error{
            std::move(right_paren), ErrorCode::kTokenNotRightParenthesis}};
      }
      return Result<NodePtr, Error>{
          std::make_unique<Group>(std::move(*expr_res.Ok()))};
    }
  }
}

} // namespace ast
} // namespace peg
} // namespace kero
