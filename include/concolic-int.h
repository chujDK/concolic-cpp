#pragma once
#include <utility>
#include <iostream>

#include "util.h"
#include "ast.h"
#include "memory"

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
      : symbolic_(AstInt::make_int()), concrete_(init_val) {}
  ConcolicInt() : symbolic_(AstInt::make_int()), concrete_(0) {}

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

  operator int() { return concrete_; }

 private:
  AstPtr symbolic_;
  int concrete_;
};

std::ostream& operator<<(std::ostream& o, const ConcolicInt& c);
