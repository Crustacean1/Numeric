#include "arithm_tests.h"
#include <string>

bool Tests::stringIdempotency(Integer &a) {
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
  if(b == a){
    return true;
  }
  std::cout<<"A: "<<a.toBin()<<" << "<<shift<<"\nB: "<<d.toBin()<<"\nC: "<<b.toBin()<<std::endl;
  return b == a;
}

bool Tests::comparision(Integer &a, Integer &b) {
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
  return d == c;
}

bool Tests::mulDivReciprocity(Integer &a, Integer &b) {
  Integer c(a.size() + b.size());

  c = a;
  c *= b;
  //std::cout << a.size() << " " << b.size() << " " << c.size() << "A: " << a
            //<< "\tB: " << b << "\tC: " << c << std::endl;

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

bool Tests::newtonDiv(Integer &a, Integer &b, Integer &c) {
  Integer d(a);
  std::cout << "And his name is: " << a << "\t" << b << std::endl;
  d /= b;
  if (d == c) {
    return true;
  }
  std::cout << "C: " << c << std::endl;
  return false;
  // std::cout<<"A: "<<a<<"\nB: "<<b<<"\nD: "<<d<<"\nC: "<<c<<std::endl;
}
