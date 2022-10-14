#pragma once
#include <utility>
#include <iostream>
#include <memory>

#include "util.h"
#include "ast.h"
#include "concolic-bool.h"

class ConcolicInt {
 public:
  // debugging..
  void z3Test();
  void astTest();

  // for debug
  virtual std::ostream& dump(std::ostream& o) const;

  ConcolicInt(const char* var_name)
      : symbolic_(AstInt::make_int(var_name)), concrete_(0) {}
  ConcolicInt(const char* var_name, const int init_val)
      : symbolic_(AstInt::make_int(var_name)), concrete_(init_val) {}
  ConcolicInt(const int init_val)
      : symbolic_(AstConstInt::make_const_int(init_val)), concrete_(init_val) {}
  ConcolicInt() : symbolic_(AstConstInt::make_const_int()), concrete_(0) {}

  ConcolicInt(AstPtr symbolic, int concrete)
      : symbolic_(std::move(symbolic)), concrete_(concrete) {}
  ConcolicInt(const ConcolicInt& concolic_int) = default;
  ConcolicInt(ConcolicInt&& concolic_int)      = default;

  ConcolicInt operator=(const ConcolicInt& concolic_int) {
    return {concolic_int.symbolic_, concolic_int.concrete_};
  }

  ConcolicInt operator=(const ConcolicInt&& concolic_int) noexcept {
    return {concolic_int.symbolic_, concolic_int.concrete_};
  }

  ConcolicInt operator+(const ConcolicInt& rhs) const {
    ConcolicInt res(AstAdd::make_add(symbolic_, rhs.symbolic_),
                    concrete_ + rhs.concrete_);
    return res;
  }

  ConcolicInt operator+(const int rhs_int) const {
    ConcolicInt rhs{rhs_int};
    return *this + rhs;
  }

  bool operator==(const ConcolicInt& rhs) const {
    auto b = ConcolicBool(AstEq::make_eq(symbolic_, rhs.symbolic_),
                          concrete_ == rhs.concrete_);
    return b;
  }

  operator int() { return concrete_; }

 private:
  AstPtr symbolic_;
  int concrete_;
};

std::ostream& operator<<(std::ostream& o, const ConcolicInt& c);
