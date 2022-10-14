#include "concolic-bool.h"

std::ostream& operator<<(std::ostream& o, const ConcolicBool& c) {
  return c.dump(o);
}
