#ifndef TESTRESULT
#define TESTRESULT

#include <cctype>

struct TestResult {
  size_t total;
  size_t passed;
  double totalTime;
  bool expected;
};

#endif /*TESTRESULT*/
