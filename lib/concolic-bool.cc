#include "concolic-bool.h"

std::ostream& operator<<(std::ostream& o, const ConcolicBool& c) {
  return c.dump(o);
}

void ConcolicBool::set_concrete(bool x) { concrete_ = x; }

std::ostream& ConcolicBool::dump(std::ostream& o) const {
  return o << "{symbolic: " << symbolic_->_z3expr() << "; "
           << "concrete: " << concrete_ << "}";
}

ConcolicBool::ConcolicBool(AstPtr symbolic, bool concrete)
    : symbolic_(std::move(symbolic)), concrete_(concrete) {
  // NOTE: symbolic has been moved to symbolic_, use that instead
  Executor::get().addConstraint(
      AstLogicAnd::make_logic_and(symbolic_,
                                  AstConstBool::make_const_bool(concrete))
          ->_z3expr());
}

ConcolicBool::operator bool() { return concrete_; }