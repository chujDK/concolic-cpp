#pragma once
#include "z3++.h"
#include "util.h"

class State {
 public:
  void addConstraint_(const z3::expr& constraint) {
    constraints_.push_back(constraint);
  }

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

class Executor {
 private:
 public:
};
