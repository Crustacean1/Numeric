#ifndef TESTS
#define TESTS

#include "../numeric/numeric.h"

namespace Tests {

using Integer = KCrypt::Numeric;

bool testStringIdempotency(Integer &a);
bool testEquality(Integer &a, Integer &b);
bool testAddition(Integer &a, Integer &b);
bool testLeftShiftLiteral(Integer &a, Integer &shift, Integer &b);
bool testComparision(Integer &a, Integer &b);

}; // namespace Tests

#endif /*TESTS*/
