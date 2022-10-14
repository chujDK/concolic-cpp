#include "util.h"

#include <cstdarg>
#include <cstddef>
#include <execinfo.h>
#include <unistd.h>
#include <string>
#include <array>
#include <iostream>

void print_backtrace() {
  std::array<void*, 10> array;
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array.data(), 10);

  // print out all the frames to stderr
  fprintf(stderr, "back trace:\n");
  backtrace_symbols_fd(array.data(), size, STDERR_FILENO);
}

const std::string generate_uniq_varname() {
  static int current = 0;
  static const std::string prefix{"tmp"};
  return prefix + std::to_string(current);
}