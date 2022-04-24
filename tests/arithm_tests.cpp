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
  std::cout<<"A: "<<a<<"\tB: "<<b<<"\tD: "<<d<<std::endl;
  return d == c;
}

bool Tests::mulDivReciprocity(Integer &a,Integer &b){
  Integer c(a.size() + b.size());
  Integer zero(a.size());
  std::cout<<"A: "<<a<<"\nB: "<<b<<std::endl;
  std::cout<<"A: "<<a.size()<<"\tB: "<<b.size()<<"\tC: "<<c.size()<<std::endl;

  c = a;
  c *= b;

  std::cout<<"C = A*B = "<<c<<std::endl;
  c /= b;

  std::cout<<"Div: "<<c<<"\t src: "<<a<<std::endl;

  return c == a;
}
