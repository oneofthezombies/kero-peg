#include "grammar_context.h"

namespace kero {
namespace peg {

GrammarContext::GrammarContext(std::string&& source) noexcept
    : source_(std::move(source)) {}

} // namespace peg
} // namespace kero
