#include "executor.h"

void Executor::addConstraint(const z3::expr& constraint) {
  State::get()->addConstraint(constraint);
}

[[nodiscard]] ConcolicInt& Executor::mk_int(const char* var_name) {
  // FIXME: thread unsafe
  concolic_ints_.emplace_back(var_name);
  return concolic_ints_.back();
}

// for debug..
const z3::expr_vector& Executor::constraint() {
  return State::get()->constraints();
}

Executor* Executor::get() {
  static Executor e;
  return &e;
}

void Executor::set_max_iter(unsigned int iter) { max_iter_ = iter; }

void Executor::findInputForCurrentConstraint() {}

Executor::Executor(unsigned int max_iter) : max_iter_(max_iter) {}