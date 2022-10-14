#include "ast.h"
#include "util.h"

std::ostream& Ast::dump(std::ostream& o) const { return o << _z3expr(); }

std::ostream& operator<<(std::ostream& o, const Ast& ast) {
  return ast.dump(o);
}

z3::expr AstAdd::_z3expr() const { return lhs_ + rhs_; }

z3::expr AstConst::_z3expr() const { return const_; }

z3::expr AstInt::_z3expr() const { return id_; }
