#ifndef TESTS
#define TESTS

#include "../numeric/numeric.h"

namespace Tests {

using Integer = KCrypt::Numeric;

bool testStringIdempotency(Integer &a);
bool testEquality(Integer &a, Integer &b);
bool testAddition(Integer &a, Integer &b);
bool testComparision(Integer &a, Integer &b);
bool testLeftShift(Integer &a, Integer &shift, Integer &b);
bool testRightShift(Integer &a, Integer &shift, Integer &b);
bool testAnyShift(Integer &a, Integer &shift);

}; // namespace Tests

#endif /*TESTS*/
