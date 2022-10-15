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
  auto a = Executor::get().mk_int("a");
  auto b = Executor::get().mk_int("b");
  Executor::get().exec(testf, a, b);
  for (auto constr : Executor::get().constraints()) {
    std::cerr << constr << std::endl;
  }

  Executor::get().findInputForConstraint(Executor::get().constraints());
  Executor::get().forceBranch(Executor::get().constraints(), 0);
}