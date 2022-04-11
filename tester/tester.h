#ifndef TESTER
#define TESTER

#include <functional>
#include <map>
#include <random>

#include "./normalizedtest.h"
#include "testcase.h"
#include "testparser.h"
#include "timer.h"

template <typename T, typename Q> class Tester {
  Timer _timer;
  Logger &_logger;

  std::map<std::string, NormalizedTest<T>> _tests;
  std::map<std::string, std::vector<TestCase<T, Q>>> _cases;
  std::default_random_engine _engine;

  template <typename... R> using TestFunc = bool (*)(R...);

  bool summary(const std ::string &name,
               const std::vector<TestResult> &results);

public:
  Tester(Logger &logger);
  template <typename... R>

  void addTest(const std::string &name, TestFunc<R...> func);
  void readStream(std::istream &stream);

  int execute();
};

template <typename T, typename Q>
Tester<T, Q>::Tester(Logger &logger) : _logger(logger), _engine(2137) {}

template <typename T, typename Q>
template <typename... R>
void Tester<T, Q>::addTest(const std::string &name, TestFunc<R...> func) {
  constexpr size_t paramCount = sizeof...(R);

  auto testFunc = [func, paramCount](std::vector<T> params) -> bool {
    return executeFunction<paramCount>(func, params);
  };
  _tests.emplace(name, NormalizedTest<T>(testFunc, paramCount));
}

template <typename T, typename Q> int Tester<T, Q>::execute() {
  bool passed = true;
  for (auto &[name, test] : _cases) {
    _logger.logInfo(3, "Executing test: ", name);
    std::vector<TestResult> caseResults;
    for (size_t i = 0; i < test.size(); ++i) {
      test[i].execute();
      caseResults.emplace_back(test[i].summary());
    }
    passed &= summary(name, caseResults);
  }
  return passed ? 0 : -1;
}

template <typename T, typename Q>
bool Tester<T, Q>::summary(const std::string &name,
                           const std::vector<TestResult> &results) {
  bool passed = true;
  for (size_t i = 0; i < results.size(); ++i) {
    if (results[i].total != results[i].passed) {
      passed = false;
      _logger.fail(1, "Test case no.", i + 1, " in test ", name, results[i].passed, " out of ",results[i].total);
    }
  }
  if (passed) {
    _logger.success(0, name, " Test suite passed");
  } else {
    _logger.fail(0, name, " Test suite failed");
  }
  return passed;
}

template <typename T, typename Q>
void Tester<T, Q>::readStream(std::istream &stream) {
  TestParser parser;
  auto syntaxTree = parser.parseStream(stream);
  for (auto &test : syntaxTree.children) {
    if (_tests.find(test.data) == _tests.end()) {
      throw std::runtime_error("Test with name: " + test.data +
                               " does not exist");
    }
    for (auto &testCase : test.children) {
      _cases[test.data].push_back(TestCase<T, Q>(
          _engine, testCase, _tests[test.data], _timer, _logger));
    }
  }
}

#endif /*TESTER*/
