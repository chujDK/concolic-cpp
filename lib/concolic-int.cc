#include <iostream>
#include <vector>

#include "executor.h"
#include "concolic-int.h"
#include "ast.h"

void ConcolicInt::z3Test() {
  std::vector<int> v;
  v.push_back(1);

  z3::context c;
  auto x = c.int_const("x");
  auto y = c.int_const("y");

  auto z = c.int_val(1);
  std::cout << z << std::endl;

  std::cout << x + y;

  z3::solver s(c);
  s.add(x * x + y * y == 25);
  s.add(x < 0 && y < 0);
  s.add(x + z == -2);
  std::cout << s.check() << std::endl;

  auto m = s.get_model();
  std::cout << m << std::endl;
  for (uint i = 0; i < m.size(); i++) {
    auto v = m[i];
    assert(v.arity() == 0);
    std::cout << v.name() << " = " << m.get_const_interp(v) << std::endl;
  }

  std::cout << "x + y + 1 = " << m.eval(x + y + 1) << std::endl;
}

void ConcolicInt::astTest() {
  auto x       = AstInt::make_int("x");
  auto y       = AstInt::make_int("y");
  auto ast_add = AstAdd::make_add(x, y);

  concolic_cpp_debug_log("ast_add:", ast_add->_z3expr());

  ConcolicInt a{"a", 20};
  ConcolicInt b{"b", 22};
  auto c  = a + b;
  int c_i = a + b;
  assert(c_i == c.concrete_);
  auto d = a + 1;
  auto e = 1 + a;

  concolic_cpp_debug_log("concolic add:\n", a, "+", b, "=", c);
  concolic_cpp_debug_log("concolic add:\n", a, "+", 1, "=", d);
  concolic_cpp_debug_log("concolic add:\n", 1, "+", a, "=", e);

  ConcolicInt n{"n", 42};
  ConcolicInt m{"m", 42};
  auto eq = n == m;
  concolic_cpp_debug_log("eq:", eq);
  for (auto constr : Executor::get()->constraint()) {
    std::cerr << constr << std::endl;
  }
}

std::ostream& ConcolicInt::dump(std::ostream& o) const {
  return o << "{symbolic: " << symbolic_->_z3expr() << "; "
           << "concrete: " << concrete_ << "}";
}

std::ostream& operator<<(std::ostream& o, const ConcolicInt& c) {
  return c.dump(o);
}
