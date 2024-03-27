#ifndef KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
#define KERO_PEG_INTERNAL_GRAMMAR_PARSER_H

#include "core.h"

namespace kero {
namespace peg {

enum class GrammarParserErrorCode : int32_t {};

class GrammarContext;
class RuleSet;

class GrammarParser {
public:
  GrammarParser(const GrammarContext& context) noexcept;
  ~GrammarParser() noexcept = default;

  GrammarParser(GrammarParser&&) = delete;
  GrammarParser(const GrammarParser&) = delete;
  auto operator=(GrammarParser&&) -> GrammarParser& = delete;
  auto operator=(const GrammarParser&) -> GrammarParser& = delete;

  auto Parse() noexcept -> Result<RuleSet, GrammarParserErrorCode>;

private:
  const GrammarContext& context_;
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
