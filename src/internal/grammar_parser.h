#ifndef KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
#define KERO_PEG_INTERNAL_GRAMMAR_PARSER_H

#include "core.h"

namespace kero {
namespace peg {

enum class ParseGrammarErrorCode : int32_t {
  kEndOfGrammarSource = 0,
  kTokenNotFound,
};

class GrammarContext;
class RuleSet;

auto ParseGrammar(const GrammarContext& context) noexcept
    -> Result<RuleSet, ParseGrammarErrorCode>;

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
