#pragma once
#include <map>
#include <set>

#include "z3++.h"
#include "util.h"
#include "concolic-int.h"

class ConcolicInt;

class Executor {
 public:
  void addConstraint(const z3::expr& constraint);

  [[nodiscard]] ConcolicInt& mk_int(const char* var_name);

  // for debug..
  const z3::expr_vector& constraints();

  static Executor& get();

  void set_max_iter(unsigned int iter);

  bool findInputForConstraint(const z3::expr_vector& constraint);

  z3::expr_vector forceBranch(const z3::expr_vector& _constraint,
                              unsigned int nth_branch);

  // seems like for the template function
  // we can't separate the definition and implementation
  template <typename FUNC, typename... ARGS>
  void exec(FUNC func, ARGS&... args) {
    current_iter_++;
    if (current_iter_ >= max_iter_) {
      return;
    }
    auto ret = func(args...);
    concolic_cpp_verbose_log("returned: ", ret, "\nexplored: ", constraints());

    z3::expr_vector old_constraints = constraints();
    auto n_constraints              = old_constraints.size();
    for (size_t i = 0; i < n_constraints; i++) {
      /*
      if (constraint_checked.find(old_constraints[i]) !=
          constraint_checked.end()) {
        continue;
      } else {
        constraint_checked.insert(old_constraints[i]);
      }
      */
      auto force_constraints = forceBranch(old_constraints, i);
      if (!findInputForConstraint(force_constraints)) {
        continue;
      }
      state.reset();
      exec(func, args...);
      if (current_iter_ >= max_iter_) {
        return;
      }
    }
  }

 private:
  Executor(unsigned int max_iter = 20) : max_iter_(max_iter) {}

  std::map<std::string, ConcolicInt> concolic_ints_;
  std::set<z3::expr> constraint_checked;
  unsigned int max_iter_;
  unsigned int current_iter_{0};

  class State {
   public:
    // TODO: store AstPtr instead of z3::expr
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

    State() : constraints_(*z3ctx) {}

   private:
    z3::expr_vector constraints_;
  } state;
};
