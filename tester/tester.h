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
  bool allClear = true;
  for (auto &[name, test] : _cases) {
    _logger.logInfo("Executing test: ", name);
    for (auto &testCase : test) {
      testCase.execute();
      allClear &= testCase.summarize();
    }
  }
  return allClear ? 0 : -1;
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
      _cases[test.data].push_back(
          TestCase<T,Q>(_engine, testCase, _tests[test.data], _timer, _logger));
    }
  }
}

#endif /*TESTER*/
