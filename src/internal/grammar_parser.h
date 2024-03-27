#ifndef KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
#define KERO_PEG_INTERNAL_GRAMMAR_PARSER_H

#include "core.h"

namespace kero {
namespace peg {

class GrammarContext;
class RuleSet;

class GrammarParser {
public:
  enum class Error : int32_t {};

  GrammarParser(const GrammarContext& context) noexcept;
  ~GrammarParser() noexcept = default;

  GrammarParser(GrammarParser&&) = delete;
  GrammarParser(const GrammarParser&) = delete;
  auto operator=(GrammarParser&&) -> GrammarParser& = delete;
  auto operator=(const GrammarParser&) -> GrammarParser& = delete;

  auto Parse() noexcept -> void;

private:
  const GrammarContext& context_;
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
