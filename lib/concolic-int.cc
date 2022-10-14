#include <iostream>
#include <vector>

#include "executor.h"
#include "concolic-int.h"
#include "ast.h"

std::ostream& ConcolicInt::dump(std::ostream& o) const {
  return o << "{symbolic: " << symbolic_->_z3expr() << "; "
           << "concrete: " << concrete_ << "}";
}

ConcolicInt::ConcolicInt(const char* var_name)
    : symbolic_(AstInt::make_int(var_name)), concrete_(0) {}
ConcolicInt::ConcolicInt(const char* var_name, const int init_val)
    : symbolic_(AstInt::make_int(var_name)), concrete_(init_val) {}
ConcolicInt::ConcolicInt(const int init_val)
    : symbolic_(AstConstInt::make_const_int(init_val)), concrete_(init_val) {}
ConcolicInt::ConcolicInt()
    : symbolic_(AstConstInt::make_const_int(0)), concrete_(0) {}

ConcolicInt::ConcolicInt(AstPtr symbolic, int concrete)
    : symbolic_(std::move(symbolic)), concrete_(concrete) {}

ConcolicInt ConcolicInt::operator=(const ConcolicInt& concolic_int) {
  return {concolic_int.symbolic_, concolic_int.concrete_};
}

ConcolicInt ConcolicInt::operator=(const ConcolicInt&& concolic_int) noexcept {
  return {concolic_int.symbolic_, concolic_int.concrete_};
}

ConcolicInt ConcolicInt::operator+(const ConcolicInt& rhs) const {
  ConcolicInt res(AstAdd::make_add(symbolic_, rhs.symbolic_),
                  concrete_ + rhs.concrete_);
  return res;
}

  ConcolicInt ConcolicInt::operator+(const int rhs_int) const {
    ConcolicInt rhs{rhs_int};
    return *this + rhs;
  }

bool ConcolicInt::operator==(const ConcolicInt& rhs) const {
  auto b = ConcolicBool(AstEq::make_eq(symbolic_, rhs.symbolic_),
                        concrete_ == rhs.concrete_);
  return b;
}

ConcolicInt::operator int() const { return concrete_; }

std::ostream& operator<<(std::ostream& o, const ConcolicInt& c) {
  return c.dump(o);
}
