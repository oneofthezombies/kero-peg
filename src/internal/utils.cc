#include "utils.h"

namespace kero {
namespace peg {

auto FindToken(const std::string_view source, const size_t position,
               const std::string_view separators) noexcept
    -> Result<FindTokenData, FindTokenErrorCode> {
  if (position >= source.size()) {
    return Result<FindTokenData, FindTokenErrorCode>{
        FindTokenErrorCode::kEndOfSource};
  }

  const auto start = source.find_first_not_of(separators, position);
  if (start == std::string_view::npos) {
    return Result<FindTokenData, FindTokenErrorCode>{
        FindTokenErrorCode::kTokenNotFound};
  }

  auto end = source.find_first_of(separators, start);
  if (end == std::string_view::npos) {
    end = source.size();
  }

  return Result<FindTokenData, FindTokenErrorCode>{
      FindTokenData{source.substr(start, end - start), end}};
}

} // namespace peg
} // namespace kero
