#pragma once
#include <utility>
#include <iostream>
#include <memory>

#include "util.h"
#include "ast.h"
#include "concolic-bool.h"

class ConcolicInt {
 public:
  // for debug
  virtual std::ostream& dump(std::ostream& o) const;

  ConcolicInt(const char* var_name);
  ConcolicInt(const char* var_name, const int init_val);
  ConcolicInt(const int init_val);
  ConcolicInt();

  ConcolicInt(AstPtr symbolic, int concrete);
  ConcolicInt(const ConcolicInt& concolic_int) = default;
  ConcolicInt(ConcolicInt&& concolic_int)      = default;

  ConcolicInt operator=(const ConcolicInt& concolic_int);
  ConcolicInt operator=(const ConcolicInt&& concolic_int) noexcept;
  ConcolicInt operator+(const ConcolicInt& rhs) const;
  ConcolicInt operator+(const int rhs_int) const;
  bool operator==(const ConcolicInt& rhs) const;
  operator int() const;

 private:
  AstPtr symbolic_;
  int concrete_;
};

std::ostream& operator<<(std::ostream& o, const ConcolicInt& c);
