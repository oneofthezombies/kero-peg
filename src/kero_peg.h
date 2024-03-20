#ifndef KERO_PEG_KERO_PEG_H
#define KERO_PEG_KERO_PEG_H

#include "kero_mpsc.h"

namespace kero {
namespace peg {
class Loader {
public:
  Loader() = default;
  ~Loader() = default;
  KERO_STRUCT_TYPE_MOVE(Loader);
};
} // namespace peg
} // namespace kero

#endif // KERO_PEG_KERO_PEG_H
