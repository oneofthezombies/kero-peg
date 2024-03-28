#include "kero_peg.h"

#include <iostream>
#include <string>

std::string kSource = "A\n"
                      "<-\n"
                      "\"Hello World!\"";

auto main() -> int {
  const kero::peg::GrammarContext context{std::move(kSource)};
  kero::peg::GrammarParser parser{context};
  auto result = parser.Parse();
  std::cout << result << std::endl;
  return 0;
}
