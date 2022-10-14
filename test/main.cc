#include "unistd.h"
#include "executor.h"
#include "concolic.h"

int testf(int a, int b) {
  if (a == b) {
    return a + b;
  } else {
    return a;
  }
}

int32_t main() {
  auto a = Executor::get()->mk_int("a");
  auto b = Executor::get()->mk_int("b");
  auto v = Executor::get()->exec(testf, a, b);
  std::cerr << v << std::endl;
  for (auto constr : Executor::get()->constraint()) {
    std::cerr << constr << std::endl;
  }
}