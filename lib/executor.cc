#include "executor.h"

#include <string>

#include "util.h"
#include "z3++.h"

void Executor::addConstraint(const z3::expr& constraint) {
  state.addConstraint(constraint);
}

[[nodiscard]] ConcolicInt& Executor::mk_int(const char* var_name) {
  std::string name{var_name};
  concolic_ints_.emplace(name, ConcolicInt{var_name});
  return concolic_ints_[name];
}

// for debug..
const z3::expr_vector& Executor::constraints() { return state.constraints(); }

Executor& Executor::get() {
  static Executor e;
  return e;
}

void Executor::set_max_iter(unsigned int iter) { max_iter_ = iter; }

bool Executor::findInputForConstraint(const z3::expr_vector& constraint) {
  auto solver = z3::solver(*z3ctx);
  solver.add(constraint);
  // TODO: fork and solve, on timeout, kill it and give up

  auto success = solver.check();
  if (success == z3::sat) {
    auto model = solver.get_model();
    concolic_cpp_verbose_log("sat\n", constraint, "\non\n", model);

    for (unsigned int i = 0; i < model.size(); i++) {
      auto concolic = model[i];
      auto concrete = model.get_const_interp(concolic);
      concolic_cpp_log("setting ", concolic.name().str(), " = ", concrete);
      // search all the concolic_*_
      auto concolic_int_iter = concolic_ints_.find(concolic.name().str());
      if (concolic_int_iter != concolic_ints_.end()) {
        concolic_int_iter->second.set_concrete(concrete.get_numeral_int());
      }
    }

    return true;
  } else {
    concolic_cpp_verbose_log("unsat on", constraint);
    return false;
  }
}

z3::expr_vector Executor::forceBranch(const z3::expr_vector& _constraint,
                                      unsigned int nth_branch) {
  if (nth_branch > _constraint.size()) {
    concolic_cpp_fatal("constraint exceed");
  }
  z3::expr_vector constraint{_constraint};
  constraint.resize(nth_branch + 1);

  auto not_branch = _constraint[nth_branch] == z3ctx->bool_val(false);

  constraint.pop_back();
  constraint.push_back(not_branch);
  concolic_cpp_debug_log("force constraint:", constraint);
  return constraint;
}
