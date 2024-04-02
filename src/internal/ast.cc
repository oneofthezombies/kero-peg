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
  if (auto res = NextToken(); res.IsErr()) {
    return Result<NodePtr, Error>{std::move(*res.Err())};
  }

  auto rules{std::vector<NodePtr>{}};
  while (true) {
    auto res = ParseRule();
    if (res.IsErr()) {
      return res;
    }
    rules.push_back(std::move(*res.Ok()));

    if (!lookahead_) {
      return Result<NodePtr, Error>{Error{ErrorCode::kLookaheadNotExist}};
    }

    if (lookahead_->kind == TokenKind::kEndOfInput) {
      break;
    }
  }

  return Result<NodePtr, Error>{std::make_unique<RuleSet>(std::move(rules))};
}

auto Parser::ParseRule() noexcept -> Result<NodePtr, Error> {
  if (auto non_terminal_res = ParseNonTerminal(); non_terminal_res.IsErr()) {
    return non_terminal_res;
  } else {
    auto non_terminal = std::move(*non_terminal_res.Ok());
    if (auto res = ConsumeLeftArrowToken(); res.IsErr()) {
      return Result<NodePtr, Error>{std::move(*res.Err())};
    }

    if (auto expr_res = ParseOrderedChoice(); expr_res.IsErr()) {
      return expr_res;
    } else {
      return Result<NodePtr, Error>{std::make_unique<Rule>(
          std::move(non_terminal), std::move(*expr_res.Ok()))};
    }
  }
}

auto Parser::ParseNonTerminal() noexcept -> Result<NodePtr, Error> {
  if (!lookahead_) {
    return Result<NodePtr, Error>{Error{ErrorCode::kLookaheadNotExist}};
  }

  if (lookahead_->kind != TokenKind::kNonTerminal) {
    return Result<NodePtr, Error>{
        Error{std::move(*lookahead_), ErrorCode::kTokenNotNonTerminal}};
  }

  auto non_terminal = std::move(*lookahead_);
  if (auto res = NextToken(); res.IsErr()) {
    return Result<NodePtr, Error>{std::move(*res.Err())};
  }

  return Result<NodePtr, Error>{
      std::make_unique<NonTerminal>(std::move(non_terminal))};
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
  if (!lookahead_) {
    return Result<NodePtr, Error>{Error{ErrorCode::kLookaheadNotExist}};
  }

  auto expr_res = ParseSequence();
  if (expr_res.IsErr()) {
    return expr_res;
  }

  while (true) {
    if (!lookahead_) {
      return Result<NodePtr, Error>{Error{ErrorCode::kLookaheadNotExist}};
    }

    if (lookahead_->kind == TokenKind::kSlash) {
      if (auto res = NextToken(); res.IsErr()) {
        return Result<NodePtr, Error>{std::move(*res.Err())};
      }

      if (auto right_expr_res = ParseSequence(); right_expr_res.IsErr()) {
        return right_expr_res;
      }

      expr_res = std::make_unique<OrderedChoice>(
          std::move(*expr_res.Ok()), std::move(*right_expr_res.Ok()));
    } else {
      break;
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

auto Parser::ParseSequence() noexcept -> Result<NodePtr, Error> {
  if (auto expr_res = ParseGroup(); expr_res.IsErr()) {
    return expr_res;
  } else {
    while (true) {
      if (!lookahead_) {
        return Result<NodePtr, Error>{Error{ErrorCode::kLookaheadNotExist}};
      }

      if (IsExpressionNodeKind(lookahead_->kind)) {
        if (auto right_expr_res = ParseGroup(); right_expr_res.IsErr()) {
          return right_expr_res;
        }

        expr_res = std::make_unique<Sequence>(std::move(*expr_res.Ok()),
                                              std::move(*right_expr_res.Ok()));
      } else {
        break;
      }
    }

    return expr_res;
  }
}

auto Parser::ParseOrderedChoice() noexcept -> Result<NodePtr, Error> {
  if (auto expr_res = ParseSequence(); expr_res.IsErr()) {
    return expr_res;
  } else {
    while (true) {
      if (!lookahead_) {
        return Result<NodePtr, Error>{Error{ErrorCode::kLookaheadNotExist}};
      }

      if (lookahead_->kind == TokenKind::kSlash) {
        if (auto res = NextToken(); res.IsErr()) {
          return Result<NodePtr, Error>{std::move(*res.Err())};
        }

        if (auto right_expr_res = ParseSequence(); right_expr_res.IsErr()) {
          return right_expr_res;
        } else {
          expr_res = std::make_unique<OrderedChoice>(
              std::move(*expr_res.Ok()), std::move(*right_expr_res.Ok()));
        }
      } else {
        break;
      }
    }

    return expr_res;
  }
}

} // namespace ast
} // namespace peg
} // namespace kero
