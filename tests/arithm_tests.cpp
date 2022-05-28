#include "arithm_tests.h"
#include <string>

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
  std::cout<<"A: "<<a<<" B: "<<b<< " D: "<<d<<" Should be: "<<c<<std::endl;
  return d == c;
}

bool Tests::mulDivReciprocity(Integer &a, Integer &b) {
  Integer c(a.size() + b.size(),0);
  
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

bool Tests::divFloor(Integer & a,Integer & b){
  Integer c(a/b);
  //std::cout<<"A: "<<a<<" B: "<<b<<" C: "<<c<<std::endl;
  Integer d(a.size());
  d = b;
  d *= c;
  a -= d;
  if(!a.isSigned() && a < b){
    return true;
  }
  return false;
}

bool Tests::nastyDivFloor(Integer &a, Integer & b){
  Integer c(a.size() + b.size());
  c = a;
  c *= b;
  c -= 1;
  Integer d(c);
  d /= a;
  //std::cout<<"A: "<<a<<" B: "<<b<<" C - 1: "<<c<<" D: "<<d<<std::endl;
  if(d == b){
    return false;
  }
  return true;
}

