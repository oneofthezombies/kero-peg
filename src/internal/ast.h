#ifndef KERO_PEG_INTERNAL_AST_H
#define KERO_PEG_INTERNAL_AST_H

#include <cassert>
#include <cstdint>
#include <memory>
#include <string_view>
#include <variant>
#include <vector>

#include "./lexer.h"

namespace kero {
namespace peg {
namespace ast {

enum class ErrorCode : int32_t {
  kTokenizeError = 0,
  kTokenNotNonTerminal,
  kTokenNotLeftArrow,
  kTokenNotRightParenthesis,
};

class Error {
public:
  Error(TokenizeError&& tokenize_error) noexcept
      : var_{std::move(tokenize_error)},
        error_code_{ErrorCode::kTokenizeError} {}
  Error(Token&& token, const ErrorCode error_code) noexcept
      : var_{token}, error_code_{error_code} {}

private:
  std::variant<TokenizeError, Token> var_;
  ErrorCode error_code_;
};

enum class NodeKind : int32_t {
  kRuleSet = 0,
  kRule,
  kSequence,
  kOrderedChoice,
  kZeroOrMore,
  kOneOrMore,
  kOptional,
  kAndPredicate,
  kNotPredicate,
  kGroup,
  kAnyCharacter,
  kNonTerminal,
  kQuotedTerminal,
  kBracketedTerminal,
};

auto IsExpressionNodeKind(const NodeKind kind) noexcept -> bool;

class Node {
public:
  virtual ~Node() noexcept = default;
  virtual auto Kind() const noexcept -> NodeKind = 0;
};

using NodePtr = std::unique_ptr<Node>;

class RuleSet : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kRuleSet;
  }

  auto AddRule(NodePtr&& rule) noexcept -> void {
    assert(rule);
    assert(rule->Kind() == NodeKind::kRule);
    rules_.push_back(rule);
  }

private:
  std::vector<NodePtr> rules_;
};

class Rule : public Node {
public:
  Rule(NodePtr&& non_terminal, NodePtr&& expression) noexcept
      : non_terminal_{std::move(non_terminal)},
        expression_{std::move(expression)} {}

  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kRule;
  }

private:
  NodePtr non_terminal_;
  NodePtr expression_;
};

class Sequence : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kSequence;
  }

  auto AddExpression(NodePtr&& expression) noexcept -> void {
    assert(expression);
    assert(IsExpressionNodeKind(expression->Kind()));
    expressions_.push_back(std::move(expression));
  }

private:
  std::vector<NodePtr> expressions_;
};

class OrderedChoice : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kOrderedChoice;
  }

  auto AddExpression(NodePtr&& expression) noexcept -> void {
    assert(expression);
    assert(IsExpressionNodeKind(expression->Kind()));
    expressions_.push_back(std::move(expression));
  }

private:
  std::vector<NodePtr> expressions_;
};

class ZeroOrMore : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kZeroOrMore;
  }

  auto SetExpression(NodePtr&& expression) noexcept -> void {
    assert(expression);
    assert(IsExpressionNodeKind(expression->Kind()));
    expression_ = std::move(expression);
  }

private:
  NodePtr expression_;
};

class OneOrMore : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kOneOrMore;
  }

  auto SetExpression(NodePtr&& expression) noexcept -> void {
    assert(expression);
    assert(IsExpressionNodeKind(expression->Kind()));
    expression_ = std::move(expression);
  }

private:
  NodePtr expression_;
};

class Optional : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kOptional;
  }

  auto SetExpression(NodePtr&& expression) noexcept -> void {
    assert(expression);
    assert(IsExpressionNodeKind(expression->Kind()));
    expression_ = std::move(expression);
  }

private:
  NodePtr expression_;
};

class AndPredicate : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kAndPredicate;
  }

  auto SetExpression(NodePtr&& expression) noexcept -> void {
    assert(expression);
    assert(IsExpressionNodeKind(expression->Kind()));
    expression_ = std::move(expression);
  }

private:
  NodePtr expression_;
};

class NotPredicate : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kNotPredicate;
  }

  auto SetExpression(NodePtr&& expression) noexcept -> void {
    assert(expression);
    assert(IsExpressionNodeKind(expression->Kind()));
    expression_ = std::move(expression);
  }

private:
  NodePtr expression_;
};

class Group : public Node {
public:
  Group(NodePtr&& expression) noexcept : expression_{std::move(expression)} {}

  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kGroup;
  }

private:
  NodePtr expression_;
};

class QuotedTerminal : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kQuotedTerminal;
  }

  auto SetValue(const std::string_view value) noexcept -> void {
    value_ = value;
  }

private:
  std::string_view value_;
};

class BracketedTerminal : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kBracketedTerminal;
  }

  auto SetValue(const std::string_view value) noexcept -> void {
    value_ = value;
  }

private:
  std::string_view value_;
};

class AnyCharacter : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kAnyCharacter;
  }
};

class NonTerminal : public Node {
public:
  NonTerminal(Token&& token) noexcept : token_{std::move(token)} {}

  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kNonTerminal;
  }

private:
  Token token_;
};

class Parser {
public:
  Parser(Lexer&& lexer) noexcept;

  auto ParseRuleSet() noexcept -> Result<NodePtr, Error>;

private:
  auto NextToken() noexcept -> Result<void, Error>;
  auto ParseRule() noexcept -> Result<NodePtr, Error>;
  auto ParseNonTerminal() noexcept -> Result<NodePtr, Error>;
  auto ConsumeLeftArrowToken() noexcept -> Result<void, Error>;
  auto ParseExpression() noexcept -> Result<NodePtr, Error>;
  auto ParseGroup() noexcept -> Result<NodePtr, Error>;

  Lexer lexer_;
  std::optional<Token> lookahead_;
};

} // namespace ast
} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_AST_H
