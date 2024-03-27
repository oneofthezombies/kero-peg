#ifndef KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
#define KERO_PEG_INTERNAL_GRAMMAR_PARSER_H

#include "core.h"
#include <string_view>

namespace kero {
namespace peg {

class GrammarContext;
class RuleSet;

using Identifier = std::string_view;
using Expression = std::string_view;
using Rule = std::pair<Identifier, Expression>;

class GrammarParser {
public:
  enum class Error : int32_t {
    kIdentifierNotFound = 0,
    kIdentifierDuplicate,
    kLeftArrowNotFound,
    kLeftArrowInvalid,
    kExpressionNotFound,
  };

  GrammarParser(const GrammarContext& context) noexcept;
  ~GrammarParser() noexcept = default;

  GrammarParser(GrammarParser&&) = delete;
  GrammarParser(const GrammarParser&) = delete;
  auto operator=(GrammarParser&&) -> GrammarParser& = delete;
  auto operator=(const GrammarParser&) -> GrammarParser& = delete;

  auto Parse() noexcept -> Result<void, Error>;

private:
  const GrammarContext& context_;
};

auto operator<<(std::ostream& os, const GrammarParser::Error& error)
    -> std::ostream&;

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_GRAMMAR_PARSER_H
