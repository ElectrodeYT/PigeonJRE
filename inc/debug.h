#pragma once

static int log_level = 2;

#define pigeon_assert(x)                                              \
  if (!(x)) {                                                         \
    std::cout << "Internal assert failed: " #x "  "                   \
	      << "\n\tFile: " << __FILE__ << ":" << __LINE__ << "\n"; \
    exit(-1);                                                         \
  }
#define pigeon_panic(x)                                                          \
  std::cout << "PANIC: " #x "\n\tFile: " << __FILE__ << ":" << __LINE__ << "\n"; \
  exit(-2);
#define pigeon_log(loc, msg) \
  if (log_level >= 1) { std::cout << "[" << loc << "] " << msg << "\n"; }
