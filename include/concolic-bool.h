#pragma once
#include <utility>
#include <iostream>

#include "util.h"
#include "executor.h"
#include "ast.h"
#include "memory"

class ConcolicBool {
 public:
  virtual std::ostream& dump(std::ostream& o) const;

  void set_concrete(bool x);

  ConcolicBool(AstPtr symbolic, bool concrete);

  operator bool();

 private:
  AstPtr symbolic_;
  bool concrete_;
};

std::ostream& operator<<(std::ostream& o, const ConcolicBool& c);
