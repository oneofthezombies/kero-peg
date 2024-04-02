#ifndef KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
#define KERO_PEG_INTERNAL_GRAMMAR_PARSER_H

#include "lexer.h"

namespace kero {
namespace peg {
namespace grammar {

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
  kQuotedTerminal,
  kBracketedTerminal,
};

class Node {
public:
  virtual ~Node() noexcept = default;
  virtual auto Kind() const noexcept -> NodeKind = 0;
};

class RuleSetNode : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kRuleSet;
  }

  auto AddRule(Node* rule) noexcept -> void { rules_.push_back(rule); }

private:
  std::vector<Node*> rules_;
};

class RuleNode : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kRule;
  }

  auto SetName(const std::string_view name) noexcept -> void { name_ = name; }

  auto SetExpression(Node* expression) noexcept -> void {
    expression_ = expression;
  }

private:
  std::string_view name_;
  Node* expression_{nullptr};
};

class SequenceNode : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kSequence;
  }

  auto AddExpression(Node* expression) noexcept -> void {
    expressions_.push_back(expression);
  }

private:
  std::vector<Node*> expressions_;
};

class OrderedChoiceNode : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kOrderedChoice;
  }

  auto AddExpression(Node* expression) noexcept -> void {
    expressions_.push_back(expression);
  }

private:
  std::vector<Node*> expressions_;
};

class ZeroOrMore : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kZeroOrMore;
  }

  auto SetExpression(Node* expression) noexcept -> void {
    expression_ = expression;
  }

private:
  Node* expression_{nullptr};
};

class OneOrMore : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kOneOrMore;
  }

  auto SetExpression(Node* expression) noexcept -> void {
    expression_ = expression;
  }

private:
  Node* expression_{nullptr};
};

class Optional : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kOptional;
  }

  auto SetExpression(Node* expression) noexcept -> void {
    expression_ = expression;
  }

private:
  Node* expression_{nullptr};
};

class AndPredicate : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kAndPredicate;
  }

  auto SetExpression(Node* expression) noexcept -> void {
    expression_ = expression;
  }

private:
  Node* expression_{nullptr};
};

class NotPredicate : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kNotPredicate;
  }

  auto SetExpression(Node* expression) noexcept -> void {
    expression_ = expression;
  }

private:
  Node* expression_{nullptr};
};

class Group : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kGroup;
  }

  auto SetExpression(Node* expression) noexcept -> void {
    expression_ = expression;
  }

private:
  Node* expression_{nullptr};
};

class QuotedTerminal : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kQuotedTerminal;
  }

  auto SetString(const std::string_view string) noexcept -> void {
    string_ = string;
  }

private:
  std::string_view string_;
};

class BracketedTerminal : public Node {
public:
  virtual auto Kind() const noexcept -> NodeKind override {
    return NodeKind::kBracketedTerminal;
  }

  auto SetString(const std::string_view string) noexcept -> void {
    string_ = string;
  }

private:
  std::string_view string_;
};

enum class ParseErrorCode : int32_t {};

class Parser {
public:
  Parser(const std::string_view source) noexcept;
  Parser(Parser&&) noexcept = default;
  ~Parser() noexcept = default;
  auto operator=(Parser&&) noexcept -> Parser& = default;

  Parser(const Parser&) = delete;
  auto operator=(const Parser&) -> Parser& = delete;

  auto Parse() noexcept -> void;

private:
  Lexer lexer_;
};

} // namespace grammar
} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
