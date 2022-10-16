#include <iostream>
#include <vector>

#include "executor.h"
#include "concolic-int.h"
#include "ast.h"

std::ostream& ConcolicInt::dump(std::ostream& o) const {
  return o << "{symbolic: " << symbolic_->_z3expr() << "; "
           << "concrete: " << concrete_ << "}";
}

void ConcolicInt::set_concrete(int x) { concrete_ = x; }
int ConcolicInt::concrete() { return concrete_; }

ConcolicInt::ConcolicInt(const char* var_name)
    : symbolic_(AstInt::make(var_name)), concrete_(0) {}
ConcolicInt::ConcolicInt(const char* var_name, const int init_val)
    : symbolic_(AstInt::make(var_name)), concrete_(init_val) {}
ConcolicInt::ConcolicInt(const int init_val)
    : symbolic_(AstConstInt::make(init_val)), concrete_(init_val) {}
ConcolicInt::ConcolicInt() : symbolic_(AstConstInt::make(0)), concrete_(0) {}

ConcolicInt::ConcolicInt(AstPtr symbolic, int concrete)
    : symbolic_(std::move(symbolic)), concrete_(concrete) {}

ConcolicInt::ConcolicInt(const ConcolicInt& concolic_int) {
  symbolic_ = concolic_int.symbolic_;
  concrete_ = concolic_int.concrete_;
}

ConcolicInt::ConcolicInt(ConcolicInt&& concolic_int) noexcept {
  symbolic_              = concolic_int.symbolic_;
  concolic_int.symbolic_ = nullptr;
  concrete_              = concolic_int.concrete_;
}

ConcolicInt& ConcolicInt::operator=(const int concrete_int) noexcept {
  symbolic_ = AstConstInt::make(concrete_int);
  concrete_ = concrete_int;
  return *this;
}

ConcolicInt ConcolicInt::operator+(const ConcolicInt& rhs) const {
  ConcolicInt res{AstAdd::make(symbolic_, rhs.symbolic_),
                  concrete_ + rhs.concrete_};
  return res;
}

  ConcolicInt ConcolicInt::operator+(const int rhs_int) const {
    ConcolicInt rhs{rhs_int};
    return *this + rhs;
  }

bool ConcolicInt::operator==(const ConcolicInt& rhs) const {
  auto b = ConcolicBool(AstEq::make(symbolic_, rhs.symbolic_),
                        concrete_ == rhs.concrete_);
  return b;
}

bool ConcolicInt::operator==(const int rhs) const {
  auto concolic_rhs = ConcolicInt(rhs);
  return *this == concolic_rhs;
}

ConcolicInt::operator int() const { return concrete_; }

std::ostream& operator<<(std::ostream& o, const ConcolicInt& c) {
  return c.dump(o);
}
