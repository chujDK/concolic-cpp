#pragma once
#include <string>
#include <vector>
#include <map>

#include "z3++.h"

void print_backtrace();

namespace {

// for debugging..
template <typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& t) {
  s << "[";
  for (std::size_t i = 0; i < t.size(); i++) {
    s << t[i] << (i == t.size() - 1 ? "" : ",");
  }
  return s << "]";
}

template <typename KEY, typename VALUE>
std::ostream& operator<<(std::ostream& s, const std::map<KEY, VALUE>& t) {
  s << "{";
  char comma = '\x00';
  for (const auto& kv : t) {
    s << comma << kv.first << ":" << kv.second;
    comma = ',';
  }
  return s << "}";
}

template <typename T, typename... T0>
void print2(T t, T0... t0) {
  std::cerr << t << " ";
  if constexpr (sizeof...(t0) > 0) {
    print2(t0...);
  } else {
    std::cerr << "\n";
  }
}

template <typename T, typename... T0>
void print2file(T t, T0... t0) {
  // TODO: replace cout to a fstream
  std::cout << t << " ";
  if constexpr (sizeof...(t0) > 0) {
    print2(t0...);
  } else {
    std::cout << "\n";
  }
}
}  // namespace

template <typename... Args>
void concolic_cpp_fatal(Args... args) {
  print2("\033[1;91m[-]\033[0m \033[91mfatal error:\033[0m", args...);

  print_backtrace();

  exit(1);
}

template <typename... Args>
void concolic_cpp_nonfatal(Args... args) {
  print2("\033[1;91m[-]\033[0m \033[91mnonfatal error:\033[0m", args...);

  print_backtrace();
}

template <typename... Args>
void concolic_cpp_log(Args... args) {
  print2("\033[1;92m[+]\033[0m", args...);
}

template <typename... Args>
void concolic_cpp_debug_log(Args... args) {
#ifdef DEBUG
  print2("\033[1;92m[+]\033[0m", args...);
#endif
}

template <typename... Args>
void concolic_cpp_verbose_log(Args... args) {
#ifdef VERBOSE
  print2("\033[1;92m[+]\033[0m", args...);
#endif
}

template <typename... Args>
void concolic_cpp_file_output(Args... args) {
  print2file(args...);
}

const std::string generate_uniq_varname();

class Z3GlobalContext {
 public:
  static z3::context* get() {
    static z3::context ctx;
    return &ctx;
  }
};

#define z3ctx Z3GlobalContext::get()