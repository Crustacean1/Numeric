#ifndef TESTER
#define TESTER

#include <functional>
#include <map>
#include <random>

#include "argument/ArgumentGeneratorFactory.h"
#include "normalizedtest.h"
#include "testcase.h"
#include "testparser.h"
#include "timer.h"

template <typename T> class Tester {
  Timer _timer;
  Logger &_logger;
  ArgumentGeneratorFactory<T> &_factory;

  std::map<std::string, NormalizedTest<T>> _tests;
  std::map<std::string, std::vector<TestCase<T>>> _cases;

  size_t _seed;

  template <typename... R> using TestFunc = bool (*)(R...);

  bool evaluate(const std ::string &name,
                const std::vector<TestResult> &results);

public:
  Tester(Logger &logger, ArgumentGeneratorFactory<T> &factory);
  template <typename... R>

  void addTest(const std::string &name, TestFunc<R...> func);
  void readStream(std::istream &stream);
  void summarize(const std::string &testName, int caseNom,
                 const TestResult &result);

  int execute();
};

template <typename T>
Tester<T>::Tester(Logger &logger, ArgumentGeneratorFactory<T> &factory)
    : _logger(logger), _factory(factory), _seed(2142) {}

template <typename T>
template <typename... R>
void Tester<T>::addTest(const std::string &name, TestFunc<R...> func) {
  constexpr size_t paramCount = sizeof...(R);

  auto testFunc = [func, paramCount](std::vector<T> params) -> bool {
    return executeFunction<paramCount>(func, params);
  };
  _tests.emplace(name, NormalizedTest<T>(testFunc, paramCount));
}

template <typename T> int Tester<T>::execute() {
  bool passed = true;
  for (auto &[name, test] : _cases) {
    _logger.logInfo(3, "Test: ", name);
    _logger.logInfo(3, "Case", "Passed", "Failed", "Total", "Time");
    std::vector<TestResult> caseResults;

    bool testResult = true;
    for (size_t i = 0; i < test.size(); ++i) {
      test[i].run();
      caseResults.emplace_back(test[i].summary());
      testResult &= (caseResults.back().passed == caseResults.back().total);
    }
    passed &= testResult;
    evaluate(name, caseResults);
    if (!testResult) {
      _logger.fail(1, "Test", name, "failed");
    } else {
      _logger.success(2, "Test", name, "passed");
    }
  }
  return passed ? 0 : -1;
}

template <typename T>
bool Tester<T>::evaluate(const std::string &name,
                         const std::vector<TestResult> &results) {
  bool passed = true;
  for (size_t i = 0; i < results.size(); ++i) {
    if (results[i].total != results[i].passed) {
      passed = false;
    }
    summarize(name, i, results[i]);
  }
  return passed;
}

template <typename T>

void Tester<T>::summarize(const std::string &testName, int caseNo,
                          const TestResult &result) {

  //_logger.logInfo(2, "Test:", testName, "Case:", caseNo);
  _logger.logInfo(3, caseNo, result.passed, (result.total - result.passed),
                  result.total, result.totalTime, "s");
}

template <typename T> void Tester<T>::readStream(std::istream &stream) {
  TestParser parser;
  auto syntaxTree = parser.parseStream(stream);
  _cases.clear();
  for (auto &test : syntaxTree.children) {
    if (_tests.find(test.data) == _tests.end()) {
      throw std::runtime_error("Test with name: " + test.data +
                               " does not exist");
    }
    for (auto &testCase : test.children) {
      _cases[test.data].push_back(TestCase<T>(
          _seed, testCase, _factory, _tests[test.data], _timer, _logger));
    }
  }
}

#endif /*TESTER*/
