#ifndef TESTS
#define TESTS

#include "../numeric/numeric.h"

namespace Tests {

using Integer = KCrypt::Numeric;

bool stringIdempotency(Integer &a);
bool equality(Integer &a, Integer &b);
bool addition(Integer &a, Integer &b);
bool subtraction(Integer &a, Integer &b, Integer &c);
bool comparision(Integer &a, Integer &b);
bool leftShift(Integer &a, Integer &shift, Integer &b);
bool rightShift(Integer &a, Integer &shift, Integer &b);
bool anyShift(Integer &a, Integer &shift);
bool basicMultiplication(Integer &a, Integer &b,Integer &c);
bool basicDivision(Integer &a,Integer &b,Integer &c);
bool mulDivReciprocity(Integer &a, Integer &b);
bool mulPerf(Integer &a, Integer &b);

}; // namespace Tests

#endif /*TESTS*/
