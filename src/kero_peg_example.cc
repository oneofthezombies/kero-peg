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
  if (result.IsError()) {
    std::cout << *result.Error() << std::endl;
    return 1;
  }

  return 0;
}
