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
  const std::vector<z3::expr>& constraints();

  static Executor& get();

  void set_max_iter(unsigned int iter);

  bool findInputForConstraint(const std::vector<z3::expr>& constraint);

  std::vector<z3::expr> forceBranch(const std::vector<z3::expr>& _constraint,
                                    unsigned int nth_branch);

  // seems like for the template function
  // we can't separate the definition and implementation
  template <typename FUNC, typename... ARGS>
  void exec(FUNC func, ARGS&... args) {
    current_iter_++;
    if (current_iter_ >= max_iter_) {
      return;
    }
    concolic_cpp_file_output("{\"input:\"", concolic_ints_, "}");
    auto ret = func(args...);
    concolic_cpp_log("returned: ", ret, "\nexplored: ", constraints());
    for (const auto& explored_constraint : constraints()) {
      constraint_checked.insert(explored_constraint.to_string());
    }

    std::vector<z3::expr> old_constraints{constraints()};
    auto n_constraints = constraints().size();
    for (size_t i = 0; i < n_constraints; i++) {
      auto force_constraints           = forceBranch(old_constraints, i);
      const auto forced_constraint_str = force_constraints[i].to_string();
      if (constraint_checked.find(forced_constraint_str) !=
          constraint_checked.end()) {
        continue;
      } else {
        constraint_checked.insert(forced_constraint_str);
      }
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
  std::set<std::string> constraint_checked;
  unsigned int max_iter_;
  unsigned int current_iter_{0};

  class State {
   public:
    // TODO: store AstPtr instead of z3::expr
    void addConstraint(const z3::expr& constraint) {
      constraints_.push_back(constraint.simplify());
    }

    const std::vector<z3::expr>& constraints() { return constraints_; }

    void reset() { constraints_.clear(); }

    void addConstraintToSolver(z3::solver& s) {
      for (const auto& c : constraints_) {
        s.add(c);
      }
    }

    State() = default;

   private:
    // NOTE: I didn't use z3::expr_vector, because i don't
    // know how to copy one expr_vector to another.
    // In some blog I found it seems the z3::expr_vector is
    // not designed to do so, so here I use std::vector
    std::vector<z3::expr> constraints_;
  } state;
};
