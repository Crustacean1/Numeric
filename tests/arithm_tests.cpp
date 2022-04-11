#include "arithm_tests.h"
#include <string>


bool Tests::testStringIdempotency(Integer &a) {
  std::string ogInt(a);

  return a == Integer(ogInt);
}

bool Tests::testEquality(Integer &a, Integer &b) { return a == b; }

bool Tests::testAddition(Integer &a, Integer &b) {
  std::cout << "a: " << a << "\tb: " << b << std::endl;
  auto c = a - b;
  std::cout << "c: " << c << std::endl;
  c += b;
  std::cout << std::endl;
  return (c == a);
}

bool Tests::testLeftShiftLiteral(Integer &a, Integer &shift, Integer &b) {
  a <<= shift;
  std::cout << "A: " << a << "\tShift: " << shift << "\tB: " << b << std::endl;
  return (b == a);
}

bool Tests::testComparision(Integer &a, Integer &b) {
  std::cout << "a: " << a << " and: " << b << std::endl;
  if (a < b || a > b) {
    if (a > b && a < b) {
      return false;
    }
    if (a == b) {
      return false;
    }
    return true;
  }
  if (a == b) {
    return true;
  }
  return false;
}
