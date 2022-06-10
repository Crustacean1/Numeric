#ifndef TESTS
#define TESTS

#include "../numeric/Numeric.h"
#include "../tester/tester.h"

template <typename T> class Tester;

namespace Tests {

using Integer = KCrypt::Numeric;

void setUpAllTests(Tester<Integer> &tester);

bool decimalConversion(Integer &a);

bool equality(Integer &a, Integer &b);
bool comparision(Integer &a, Integer &b);

bool addition(Integer &a, Integer &b);
bool subtraction(Integer &a, Integer &b, Integer &c);

bool leftShift(Integer &a, Integer &shift, Integer &b);
bool rightShift(Integer &a, Integer &shift, Integer &b);
bool anyShift(Integer &a, Integer &shift);
bool rightShiftComplement(Integer &a, Integer &shift, Integer &c);

bool mulPerf(Integer &a, Integer &b);
bool basicMultiplication(Integer &a, Integer &b, Integer &c);

bool basicDivision(Integer &a, Integer &b, Integer &c);
bool divFloor(Integer &a, Integer &b);
bool nastyDivFloor(Integer &a, Integer &b);

bool mulDivReciprocity(Integer &a, Integer &b);

bool smallModulo(Integer &source, Integer &modulo, Integer &result);

bool modExponentValue(Integer &base, Integer &exponent, Integer &modulo,
                      Integer &expected);

bool extGcdValue(Integer &a, Integer &b, Integer &d);

bool millerRabin(Integer &suspect, Integer &witness);

bool keyGeneration(Integer & size);

}; // namespace Tests

#endif /*TESTS*/
