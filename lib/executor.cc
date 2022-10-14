#include <string>

#include "util.h"
#include "z3++.h"
#include "executor.h"

void Executor::addConstraint(const z3::expr& constraint) {
  State::get()->addConstraint(constraint);
}

[[nodiscard]] ConcolicInt& Executor::mk_int(const char* var_name) {
  std::string name{var_name};
  concolic_ints_.emplace(name, ConcolicInt{var_name});
  return concolic_ints_[name];
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

bool Executor::findInputForConstraint(const z3::expr_vector& constraint) {
  auto solver = z3::solver(*z3ctx);
  solver.add(constraint);
  // fork and solve, on timeout, kill it and give up

  auto success = solver.check();
  if (success == z3::sat) {
    auto model = solver.get_model();
    concolic_cpp_verbose_log("sat\n", constraint, "\non\n", model);

    for (unsigned int i = 0; i < model.size(); i++) {
      auto v = model[i];
      // search all the concolic_*_
      ConcolicInt& concolic_int = concolic_ints_[v.name().str()];
      concolic_int.set_concrete(model.get_const_interp(v));
    }

    return true;
  } else {
    concolic_cpp_verbose_log("unsat on", constraint);
    return false;
  }
}

Executor::Executor(unsigned int max_iter) : max_iter_(max_iter) {}