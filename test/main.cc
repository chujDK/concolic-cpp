#include "concolic.h"

int32_t main() {
  int a("a", 1);
  a.astTest();

  return 0;
}