#include "grammar_parser.h"

#include "grammar_context.h"
#include <sstream>
#include <string_view>

namespace kero {
namespace peg {

GrammarParser::GrammarParser(const GrammarContext& context) noexcept
    : context_(context) {}

auto GrammarParser::Parse() noexcept
    -> Result<RuleSet, GrammarParserErrorCode> {
  return Result<RuleSet, GrammarParserErrorCode>{};
}

} // namespace peg
} // namespace kero
