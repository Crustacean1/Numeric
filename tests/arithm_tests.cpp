#include "arithm_tests.h"
#include "../tester/tester.h"
#include <iostream>
#include <string>

void Tests::setUpAllTests(Tester<Integer> &tester) {
  tester.addTest("Equality", equality);
  tester.addTest("ComparisionSelfTest", comparision);

  tester.addTest("AdditionSelfTest", addition);
  tester.addTest("SubtractionSelfTest", subtraction);

  tester.addTest("LeftShiftValueTest", leftShift);
  tester.addTest("RightShiftValueTest", rightShift);
  tester.addTest("ShiftSelfTest", anyShift);
  tester.addTest("NegativeShiftComplementTest", rightShiftComplement);

  tester.addTest("DivisionValueTest", basicDivision);
  tester.addTest("MultiplicationDivisionSelfTest", mulDivReciprocity);
  tester.addTest("MulPerformanceTest", mulPerf);
  tester.addTest("DivisionFloorTest", divFloor);
  tester.addTest("NastyDivisionFloorTest", nastyDivFloor);

  tester.addTest("ModularExponentiationValueTest", modExponentValue);

  tester.addTest("GcdValueTest", extGcdValue);
}

bool Tests::decimalConversion(Integer &a) {
  std::string ogInt(a);

  return a == Integer(ogInt);
}

bool Tests::equality(Integer &a, Integer &b) { return a == b; }

bool Tests::addition(Integer &a, Integer &b) {
  Integer c = a - b;
  auto d = c;
  c += b;

  if (c == a) {
    return true;
  }
  std::cout << "A: " << a << "\tB: " << b << "\tC: " << c << "\tD: " << d
            << std::endl;
  return false;
}
bool Tests::subtraction(Integer &a, Integer &b, Integer &c) {
  auto d = a;
  d -= b;
  if (d == c) {
    return true;
  }
  std::cout << "D: " << d << "A: " << a << "B: " << b << std::endl;
  return false;
}

bool Tests::leftShift(Integer &a, Integer &shift, Integer &b) {
  a <<= shift;
  return (b == a);
}
bool Tests::rightShift(Integer &a, Integer &shift, Integer &b) {
  a >>= shift;
  return (b == a);
}

bool Tests::anyShift(Integer &a, Integer &shift) {
  Integer b(a.size() * 2);
  b = a;
  b <<= shift;
  auto d = b;
  b >>= shift;
  if (b == a) {
    return true;
  }
  std::cout << "A: " << a.toBin() << " << " << shift << "\nB: " << d.toBin()
            << "\nC: " << b.toBin() << std::endl;
  return b == a;
}

bool Tests::rightShiftComplement(Integer &source, Integer &shift,
                                 Integer &target) {
  source.inverse();
  target.inverse();
  source >>= shift;
  if (source == target) {
    return true;
  }
  std::cout << "Source: " << source.toBin() << "\tTarget: " << target
            << std::endl;
  return false;
}

bool Tests::comparision(Integer &a, Integer &b) {
  // std::cout<<" "<<(a>b)<<" "<<(b>a)<<" "<<(a==b)<<std::endl;
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

bool Tests::basicMultiplication(Integer &a, Integer &b, Integer &c) {
  auto d = a * b;
  return d == c;
}

bool Tests::basicDivision(Integer &a, Integer &b, Integer &c) {
  auto d = a / b;
  // std::cout << "A: " << a << " B: " << b << " D: " << d << " Should be: " <<
  // c
  //<< std::endl;
  return d == c;
}

bool Tests::mulDivReciprocity(Integer &a, Integer &b) {
  Integer c(a.size() + b.size(), 0);

  c = a;
  c *= b;

  c /= b;

  if (c == a) {
    return true;
  }
  std::cout << a.size() << " " << b.size() << " " << c.size() << "A: " << a
            << "\tB: " << b << "\tC: " << c << std::endl;

  return false;
}
bool Tests::mulPerf(Integer &a, Integer &b) {
  Integer c(a.size() + b.size());
  c = a;
  c *= a;
  return true;
}

bool Tests::divFloor(Integer &a, Integer &b) {
  Integer c(a / b);
  Integer d(a.size());
  d = b;
  d *= c;
  a -= d;
  if (!a.isSigned() && a < b) {
    return true;
  }
  return false;
}

bool Tests::nastyDivFloor(Integer &a, Integer &b) {
  Integer c(a.size() + b.size());
  c = a;
  c *= b;

  Integer d(c);
  d /= a;

  if (!(d == b)) {
    return false;
  }

  d = c;
  d -= 1;

  d /= a;
  if (d == b) {
    return false;
  }
  return true;
}

bool Tests::modExponentValue(Integer &base, Integer &exponent, Integer &modulo,
                             Integer &expected) {
  Integer d(modulo.modExp(base, exponent));
  if(d==expected){return true;}
  std::cout<<"Test: modExponentValue failed:"<<std::endl;
  std::cout << "Base: " << base << " Exponent: " << exponent
            << " Modulo: " << modulo << " Result: " << d << std::endl;
    return false;
}

bool Tests::extGcdValue(Integer &a, Integer &b, Integer &c) {
  auto [coe1, coe2] = a.extGcd(b);

  if (coe1.isSigned() == coe2.isSigned()) {
    return false;
  }
  coe1.abs();
  coe2.abs();

  Integer d(a.size() + b.size());
  Integer e(a.size() + b.size());
  d = a;
  d *= coe2;
  e = b;
  e *= coe1;

  Integer result(d.size());
  result = d;
  result -= e;
  result.abs();
  if (result == c) {
    return true;
  }

  std::cout << "Test: extGcdValue failed: " << std::endl;
  std::cout << "Result: " << result << " D: " << d << " E: " << e << std::endl;
  return false;
}
