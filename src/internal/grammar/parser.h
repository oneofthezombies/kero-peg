#ifndef KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
#define KERO_PEG_INTERNAL_GRAMMAR_PARSER_H

#include "lexer.h"

namespace kero {
namespace peg {
namespace grammar {

enum class NodeKind : int32_t {
  kNonTerminal = 0,
  kTerminal,
  kSequence,
  kOrderedChoice,
  kZeroOrMore,
  kOneOrMore,
  kOptional,
  kAndPredicate,
  kNotPredicate,
  kGroup,
};

struct Node {
  Token token;
  NodeKind kind;
};

using RuleId = int32_t;

class Symbol {
public:
  auto IsTerminal() const noexcept -> bool;
  auto IsNonTerminal() const noexcept -> bool;

  auto NonTerminal() const noexcept -> RuleId;
};

using Sequence = std::vector<Symbol>;

class Rule {
public:
  auto Choices() const noexcept -> const std::vector<Sequence>&;

private:
  std::vector<Sequence> choices_;
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
