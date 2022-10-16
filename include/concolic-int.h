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

  void set_concrete(int x);
  int concrete();

  ConcolicInt(const char* var_name);
  ConcolicInt(const char* var_name, const int init_val);
  ConcolicInt(const int init_val);
  ConcolicInt();
  virtual ~ConcolicInt() = default;

  ConcolicInt(AstPtr symbolic, int concrete);
  ConcolicInt(const ConcolicInt& concolic_int);
  ConcolicInt(ConcolicInt&& concolic_int) noexcept;

  ConcolicInt& operator=(const ConcolicInt& concolic_int) = default;
  ConcolicInt& operator=(ConcolicInt&& concolic_int)      = default;
  ConcolicInt& operator=(const int concrete_int) noexcept;

  ConcolicInt operator+(const ConcolicInt& rhs) const;
  ConcolicInt operator+(const int rhs_int) const;
  friend ConcolicInt operator+(const int lhs, const ConcolicInt& rhs);

  bool operator==(const ConcolicInt& rhs) const;
  bool operator==(const int rhs) const;
  friend bool operator==(const int lhs, const ConcolicInt& rhs);

  bool operator!=(const ConcolicInt& rhs) const;
  bool operator!=(const int rhs) const;
  friend bool operator!=(const int lhs, const ConcolicInt& rhs);
  operator int() const;

 private:
  AstPtr symbolic_;
  int concrete_;
};
ConcolicInt operator+(const int lhs, const ConcolicInt& rhs);
bool operator==(const int lhs, const ConcolicInt& rhs);
bool operator!=(const int lhs, const ConcolicInt& rhs);

std::ostream& operator<<(std::ostream& o, const ConcolicInt& c);
