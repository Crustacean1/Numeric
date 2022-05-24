#ifndef TESTCASE
#define TESTCASE

#include <functional>
#include <random>
#include <vector>

#include "argument/ArgumentGenerator.h"
#include "argument/ArgumentGeneratorFactory.h"
#include "logger.h"
#include "normalizedtest.h"
#include "syntaxnode.h"
#include "testresult.h"
#include "timer.h"

template <typename T> class TestCase {

  void createArgumentTemplate(ArgumentGeneratorFactory<T> &factory,
                              SyntaxNode &caseNode);
  void deleteArgumentTemplate();

  std::vector<T> createArguments(std::default_random_engine &e);

  TestResult readCaseData(SyntaxNode &caseNode);

  TestResult _result;

  size_t _seed;

  NormalizedTest<T> &_test;
  Logger &_logger;
  Timer &_timer;

  std::vector<ArgumentGenerator<T> *> _argTemplate;

  void displayFailInfo(const std::vector<T> &args, size_t i);
  void run(size_t i);
  TestCase(const TestCase &);

public:
  TestCase(size_t seed, SyntaxNode &caseNode,
           ArgumentGeneratorFactory<T> &factory, NormalizedTest<T> &test,
           Timer &timer, Logger &logger);
  TestCase(TestCase &&);
  ~TestCase();
  void run();
  TestResult summary();
};

template <typename T>
TestCase<T>::TestCase(size_t seed, SyntaxNode &caseNode,
                      ArgumentGeneratorFactory<T> &factory,
                      NormalizedTest<T> &test, Timer &timer, Logger &logger)
    : _result(readCaseData(caseNode)), _test(test), _seed(seed),
      _logger(logger), _timer(timer) {
  createArgumentTemplate(factory, caseNode);
}

template <typename T>
TestCase<T>::TestCase(TestCase<T> &&testCase)
    : _test(testCase._test), _logger(testCase._logger), _timer(testCase._timer),
      _seed(testCase._seed), _result(testCase._result) {
  deleteArgumentTemplate();

  for (auto arg : testCase._argTemplate) {
    _argTemplate.push_back(arg);
  }
  testCase._argTemplate.clear();
}

template <typename T> void TestCase<T>::run(size_t i) {
  std::default_random_engine engine(_seed + i);
  auto arguments = createArguments(engine);

  _timer.start();
  auto passed = _test.execute(arguments);
  _result.totalTime += _timer.read();
  _result.passed += (passed == _result.expected);

  if (passed != _result.expected) {
    displayFailInfo(arguments, i);
  }
}

template <typename T> void TestCase<T>::run() {
  for (size_t i = 0; i < _result.total; ++i) {
    run(i);
  }
  _result.totalTime /= 1000.0; // to seconds
  _result.totalTime /= _result.total;
}

template <typename T>
void TestCase<T>::displayFailInfo(const std::vector<T> &args, size_t i) {
  _logger.fail(1, "Test failed for case:", i, "with seed:", _seed + i);
  for (int i = 0; i < args.size(); ++i) {
    _logger.fail(1, "Argument", i, args[i]);
  }
}

template <typename T> TestResult TestCase<T>::summary() { return _result; }

template <typename T>
void TestCase<T>::createArgumentTemplate(ArgumentGeneratorFactory<T> &factory,
                                         SyntaxNode &mainNode) {
  for (auto &argument : mainNode.children) {
    try {
      _argTemplate.push_back(factory.create(argument));
    } catch (std::exception &e) {
      _logger.logError(0, std::string("Failed to parse argument from file: ") +
                              e.what());
    }
  }
}

template <typename T> TestResult TestCase<T>::readCaseData(SyntaxNode &node) {
  if (node.data == "assert" || node.data == "!assert") {
    if (node.data == "assert") {
      return {1, 0, 0, true};
    }
    return {1, 0, 0, false};
  }
  return {(size_t)std::atoi(node.data.c_str()), 0, 0, true};
}

template <typename T>
std::vector<T> TestCase<T>::createArguments(std::default_random_engine &e) {
  std::vector<T> result;
  for (size_t i = 0; i < _argTemplate.size(); ++i) {
    result.emplace_back(_argTemplate[i]->createInstance(e));
  }
  return result;
}
template <typename T> TestCase<T>::~TestCase() { deleteArgumentTemplate(); }

template <typename T> void TestCase<T>::deleteArgumentTemplate() {
  for (size_t i = 0; i < _argTemplate.size(); ++i) {
    delete _argTemplate[i];
  }
}


#endif /*TESTCASE*/
