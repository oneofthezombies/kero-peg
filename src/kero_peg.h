#ifndef KERO_PEG_KERO_PEG_H
#define KERO_PEG_KERO_PEG_H

#include "kero_mpsc.h"

namespace kero {
namespace peg {

enum class ExpressionKind : int32_t {
  kUnknown = 0,
  kTerminal,
  kSequence,
  kOrderedChoice,
  kAndPredicate,
  kNotPredicate,
  kOneOrMore,
  kZeroOrMore,
  kOptional,
  kGroup,
  kTerminalRange,
  kAnyCharacter,
};

class PackratParser {
public:
  auto Parse(std::string&& input) -> void;

private:
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_KERO_PEG_H
