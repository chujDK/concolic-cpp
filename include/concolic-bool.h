#pragma once
#include <utility>
#include <iostream>

#include "util.h"
#include "executor.h"
#include "ast.h"
#include "memory"

class ConcolicBool {
 public:
  virtual std::ostream& dump(std::ostream& o) const {
    return o << "{symbolic: " << symbolic_->_z3expr() << "; "
             << "concrete: " << concrete_ << "}";
  }

  ConcolicBool(AstPtr symbolic, bool concrete)
      : symbolic_(std::move(symbolic)), concrete_(concrete) {
    // NOTE: symbolic has been moved, use symbolic_ instead
    Executor::get()->addConstraint(
        AstLogicAnd::make_logic_and(symbolic_,
                                    AstConstBool::make_const_bool(concrete))
            ->_z3expr());
  }

  operator bool() { return concrete_; }

 private:
  AstPtr symbolic_;
  bool concrete_;
};

std::ostream& operator<<(std::ostream& o, const ConcolicBool& c);
