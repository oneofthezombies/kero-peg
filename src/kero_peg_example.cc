#include "kero_log.h"

auto main() -> int {
  kero::log::GlobalCenter().SetLogLevel(kero::log::Level::kDebug);
  kero::log::GlobalCenter().AddTransport(
      std::make_unique<kero::log::ConsoleTransport>());
  kero::log::Debug("Hello, world!").Log();
  kero::log::GlobalCenter().Shutdown();
  return 0;
}
