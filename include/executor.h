#pragma once
#include "z3++.h"
#include "util.h"

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

class Executor {
 public:
  void addConstraint(const z3::expr& constraint) {
    State::get()->addConstraint(constraint);
  }

  // for debug..
  const z3::expr_vector& constraint() { return State::get()->constraints(); }

  static Executor* get() {
    static Executor e;
    return &e;
  }

  void set_max_iter(unsigned int iter) { max_iter_ = iter; }

  void findInputForCurrentConstraint() {}

  // seems like for the template function
  // we can't separate the definition and implementation
  template <typename FUNC, typename... ARGS>
  auto exec(FUNC func, ARGS... args) {
    auto v = func(args...);
    return v;
  }

 private:
  Executor(unsigned int max_iter = 2000) : max_iter_(max_iter) {}
  unsigned int max_iter_;
};
