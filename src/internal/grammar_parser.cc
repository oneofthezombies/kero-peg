#include "grammar_parser.h"

#include "grammar_context.h"
#include <sstream>
#include <string_view>

namespace kero {
namespace peg {

GrammarParser::GrammarParser(const GrammarContext& context) noexcept
    : context_{context} {}

auto GrammarParser::Parse() noexcept -> Result<RuleSet> {
  Position pos{};
  while (auto token_res = NextToken(pos)) {
    if (!token_res) {
      return core::Error<ErrorCode>{ErrorCode::kEndOfGrammarSource};
    }

    const auto& token = token_res->token;
    pos = token_res->next_pos;
  }

  auto res = Foo();
  if (res.IsOk()) {
    return core::Ok<RuleSet>{std::move(res).Take()};
  }
}

} // namespace peg
} // namespace kero
