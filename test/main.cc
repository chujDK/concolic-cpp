#include "unistd.h"
#include "executor.h"
#include "concolic.h"

int testf(int a, int b) {
  if (a == b) {
    return a + b;
  } else {
    return a + 3;
  }
}

int32_t main() {
  auto& a = Executor::get().mk_int("a");
  auto& b = Executor::get().mk_int("b");

  Executor::get().exec(testf, a, b);
}