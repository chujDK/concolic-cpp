#include "unistd.h"
#include "concolic.h"

int testf(int a, int b) {
  if (a == b) {
    return a + b;
  } else {
    return a;
  }
}

int32_t main() {
  ConcolicInt a{"a", 5};
  ConcolicInt b{"b", 7};

  auto v = Executor::get()->exec(testf, a, b);
  std::cerr << v << std::endl;
  for (auto constr : Executor::get()->constraint()) {
    std::cerr << constr << std::endl;
  }
}