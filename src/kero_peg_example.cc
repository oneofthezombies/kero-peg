#include "kero_log.h"
#include "kero_peg.h"

auto main() -> int {
  using namespace kero::peg;
  const auto expr = Sequence{{
      TerminalCharacter{'a'},
      TerminalCharacter{'b'},
      TerminalCharacter{'c'},
  }};
  return 0;
}
