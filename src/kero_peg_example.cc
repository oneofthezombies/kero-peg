#include "kero_peg.h"

#include <string>

std::string kSource = "A\n"
                      "<-\n"
                      "\"Hello World!\"";

auto main() -> int {
  const kero::peg::GrammarContext context{std::move(kSource)};
  kero::peg::GrammarParser parser{context};
  parser.Parse();
  return 0;
}
