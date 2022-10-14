#pragma once
#include <map>

#include "z3++.h"
#include "util.h"
#include "concolic-int.h"

namespace {
class State {
 public:
  void addConstraint(const z3::expr& constraint) {
    constraints_.push_back(constraint);
  }

  const z3::expr_vector& constraints() { return constraints_; }

  void reset() { constraints_.resize(0); }

  void addConstraintToSolver(z3::solver& s) {
    for (const auto& c : constraints_) {
      s.add(c);
    }
  }

  static State* get() {
    static State s;
    return &s;
  }

 private:
  State() : constraints_(*z3ctx) {}

  z3::expr_vector constraints_;
};
}  // namespace

class ConcolicInt;

class Executor {
 public:
  void addConstraint(const z3::expr& constraint);

  [[nodiscard]] ConcolicInt& mk_int(const char* var_name);

  // for debug..
  const z3::expr_vector& constraint();

  static Executor* get();

  void set_max_iter(unsigned int iter);

  bool findInputForConstraint(const z3::expr_vector& constraint);

  // seems like for the template function
  // we can't separate the definition and implementation
  template <typename FUNC, typename... ARGS>
  auto exec(FUNC func, ARGS... args) {
    auto v = func(args...);
    return v;
  }

 private:
  std::map<std::string, ConcolicInt> concolic_ints_;
  Executor(unsigned int max_iter = 2000);
  unsigned int max_iter_;
};
