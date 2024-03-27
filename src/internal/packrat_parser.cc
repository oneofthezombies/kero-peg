#include <cstddef>
#include <string_view>

namespace kero {
namespace peg {

struct Rule {};
struct Position {};
struct Entry {};

auto getMemo(Rule rule, Position position) -> Entry;
auto setMemo() -> void;

auto input(size_t position, std::string_view input) -> std::string_view;
auto rule(Rule rule, size_t position) -> Entry;
auto evaluate(Rule rule, size_t position) -> Entry {
  auto start = position;
    for
      choice in rule.choices {
        auto acc = 0;
        for
          symbol in choice {
            if symbol
              .IsTerminal() {
                if input (start + acc)
                  == symbol.terminal { acc += 1; }
                else {
                  break;
                }
              }
            else {
              auto result = rule(symbol.nonterminal, start + acc);
            }
          }
      }
}

} // namespace peg
} // namespace kero
