#ifndef KERO_PEG_INTERNAL_UTILS_H
#define KERO_PEG_INTERNAL_UTILS_H

#include "core.h"
#include <string_view>

namespace kero {
namespace peg {

enum class FindTokenErrorCode : int32_t {
  kEndOfSource = 0,
  kTokenNotFound,
};

struct FindTokenResult {
  std::string_view token;
  size_t next_position;
};

auto FindToken(const std::string_view source, const size_t position,
               const std::string_view separators = " \t\n") noexcept
    -> Result<FindTokenResult, FindTokenErrorCode>;

} // namespace peg
} // namespace kero

#endif // KERO_PEG_INTERNAL_UTILS_H
