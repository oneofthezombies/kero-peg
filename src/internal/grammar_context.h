#ifndef KERO_PEG_INTERNAL_GRAMMAR_CONTEXT_H
#define KERO_PEG_INTERNAL_GRAMMAR_CONTEXT_H

#include <string>

namespace kero {
namespace peg {

class GrammarContext {
public:
  GrammarContext(std::string&& source) noexcept;
  ~GrammarContext() noexcept = default;

  GrammarContext(GrammarContext&&) = delete;
  GrammarContext(const GrammarContext&) = delete;
  auto operator=(GrammarContext&&) -> GrammarContext& = delete;
  auto operator=(const GrammarContext&) -> GrammarContext& = delete;

  auto Source() const noexcept -> std::string_view;

private:
  std::string source_;
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_GRAMMAR_CONTEXT_H
