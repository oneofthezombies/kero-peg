#include "grammar_parser.h"

#include "grammar_context.h"
#include <sstream>
#include <string_view>

namespace kero {
namespace peg {

auto ParseGrammar(const GrammarContext& context) noexcept
    -> Result<RuleSet, ParseGrammarErrorCode> {}

} // namespace peg
} // namespace kero
