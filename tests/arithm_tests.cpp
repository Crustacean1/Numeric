#include "arithm_tests.h"
#include <string>

bool Tests::stringIdempotency(Integer &a) {
  std::string ogInt(a);

  return a == Integer(ogInt);
}

bool Tests::equality(Integer &a, Integer &b) { return a == b; }

bool Tests::addition(Integer &a, Integer &b) {
  auto c = a - b;
  c += b;
  return (c == a);
}
bool Tests::subtraction(Integer &a,Integer &b,Integer &c){
  auto d = a;
  d -= b;
  std::cout<<"D: "<<d<<"A: "<<a<<"B: "<<b<<std::endl;
  return (d==c);
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
  b >>= shift;
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

bool Tests::basicDivision(Integer &a,Integer &b,Integer &c){
  auto d = a/b;
  return d == c;
}

bool Tests::mulDivReciprocity(Integer &a,Integer &b){
  Integer c(a.size() + b.size());
  Integer zero(a.size());

  c = a;
  c *= b;
  Integer d(c);

  c /= b;

  if(c == a){
    return true;
  }
  std::cout<<"A: "<<a<<"\tB: "<<b<<"\tC: "<<d<<std::endl;
  return false;
}
