#include "grammar_context.h"

namespace kero {
namespace peg {

GrammarContext::GrammarContext(std::string&& source) noexcept
    : source_(std::move(source)) {}

auto GrammarContext::Source() const noexcept -> std::string_view {
  return source_;
}

} // namespace peg
} // namespace kero
