#ifndef KERO_PEG_INTERNAL_PACKRAT_H
#define KERO_PEG_INTERNAL_PACKRAT_H

#include "./core.h"
#include <functional>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace kero {
namespace peg {

enum class PackratErrorCode : int32_t {
  kRuleAlreadyExists = 0,
  kRuleNotFound,
};

class Expression {};

class Sequence {
public:
  Sequence() noexcept = default;
  ~Sequence() noexcept = default;

  auto AddExpression(Expression&& expression) noexcept -> void {
    expressions_.push_back(std::move(expression));
  }

  auto GetExpressions() const noexcept -> const std::vector<Expression>& {
    return expressions_;
  }

private:
  std::vector<Expression> expressions_;
};

class OrderedChoice {
public:
  OrderedChoice() noexcept = default;
  ~OrderedChoice() noexcept = default;

  auto SetSequence(Sequence&& sequence) noexcept -> void {
    sequence_ = std::move(sequence);
  }

  auto GetSequence() const noexcept -> const Sequence& { return sequence_; }

private:
  Sequence sequence_;
};

class Rule {
public:
  Rule() noexcept = default;
  ~Rule() noexcept = default;

  auto AddOrderedChoice(OrderedChoice&& ordered_choice) noexcept -> void {
    ordered_choices_.push_back(std::move(ordered_choice));
  }

  auto GetOrderedChoices() const noexcept -> const std::vector<OrderedChoice>& {
    return ordered_choices_;
  }

private:
  std::vector<OrderedChoice> ordered_choices_;
};

using RuleName = std::string_view;

class RuleSet {
public:
  auto AddRule(const RuleName name, Rule&& rule) noexcept
      -> Result<void, PackratErrorCode> {
    if (rule_map_.find(name) != rule_map_.end()) {
      return PackratErrorCode::kRuleAlreadyExists;
    }

    rule_map_.emplace(name, std::move(rule));
    return {};
  }

  auto TryGetRule(const RuleName name) const noexcept
      -> Result<std::reference_wrapper<const Rule>, PackratErrorCode> {
    auto it = rule_map_.find(name);
    if (it == rule_map_.end()) {
      return PackratErrorCode::kRuleNotFound;
    }

    return std::ref(it->second);
  }

private:
  std::unordered_map<RuleName, Rule> rule_map_;
};

class PackratParser {
public:
  PackratParser() noexcept = default;
  ~PackratParser() noexcept = default;

  auto Parse(const RuleSet& rule_set, const RuleName start_rule_name) noexcept
      -> void {}
};

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_PACKRAT_H
