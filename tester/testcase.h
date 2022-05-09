#ifndef TESTCASE
#define TESTCASE

#include <functional>
#include <random>
#include <vector>

#include "logger.h"
#include "normalizedtest.h"
#include "syntaxnode.h"
#include "testresult.h"
#include "timer.h"

template <typename T, typename Q> class TestCase {
  using TestArgumentGenerator = std::function<T()>;

  TestArgumentGenerator createSigned(const std::string &size);
  TestArgumentGenerator createUnsigned(const std::string &size);
  TestArgumentGenerator createRandom(const std::string &size);
  TestArgumentGenerator createRange(const std::string &min,
                                    const std::string &max);
  TestArgumentGenerator createValue(const std::string &value);

  std::vector<TestArgumentGenerator>
  createArgumentGenerator(SyntaxNode &caseNode);
  TestResult readCaseData(SyntaxNode &caseNode);

  std::vector<T> createArguments();

  TestResult _result;

  std::default_random_engine &_engine;

  NormalizedTest<T> &_test;
  Logger &_logger;
  Timer &_timer;

  const std::vector<TestArgumentGenerator> _argumentGenerator;

public:
  TestCase(std::default_random_engine &engine, SyntaxNode &caseNode,
           NormalizedTest<T> &test, Timer &timer, Logger &logger);
  void execute();
  TestResult summary();
};

template <typename T, typename Q>
TestCase<T, Q>::TestCase(std::default_random_engine &engine,
                         SyntaxNode &caseNode, NormalizedTest<T> &test,
                         Timer &timer, Logger &logger)
    : _result(readCaseData(caseNode)), _engine(engine), _test(test),
      _logger(logger), _timer(timer),
      _argumentGenerator(createArgumentGenerator(caseNode)) {}

template <typename T, typename Q> void TestCase<T, Q>::execute() {
  for (size_t i = 0; i < _result.total; ++i) {
    auto arguments = createArguments();
    _timer.start();
    auto passed = _test.execute(arguments);
    _result.totalTime += _timer.read();
    if (passed == _result.expected) {
      ++_result.passed;
    }
  }
  _result.totalTime /= (1000*_result.total);
}

template <typename T, typename Q> TestResult TestCase<T, Q>::summary() {
  if (_result.total != _result.passed) {
    _logger.fail(2, "\t", _result.passed, "out of", _result.total,
                 "failed:", (_result.total - _result.passed),
                 "with avg time:", _result.totalTime, "s");
  } else {
    _logger.success(2, "\t", _result.passed, "out of", _result.total,
                    "failed:", (_result.total - _result.passed),
                    "with avg time:", _result.totalTime, "s");
  }
  return _result;
}

template <typename T, typename Q>
std::vector<std::function<T()>>
TestCase<T, Q>::createArgumentGenerator(SyntaxNode &mainNode) {
  std::vector<std::function<T()>> arguments;

  for (auto &argument : mainNode.children) {
    if (argument.data == "signed") {
      arguments.push_back(createSigned(argument.children[0].data));
    } else if (argument.data == "unsigned") {
      arguments.push_back(createUnsigned(argument.children[0].data));
    } else if (argument.data == "random") {
      arguments.push_back(createRandom(argument.children[0].data));
    } else if (argument.data == "range") {
      arguments.push_back(
          createRange(argument.children[0].data, argument.children[1].data));
    } else if (argument.data == "value") {
      arguments.push_back(createValue(argument.children[0].data));
    } else {
      throw std::runtime_error("Encountered unknown operand: " + argument.data);
    }
  }
  return arguments;
}

template <typename T, typename Q>
std::vector<T> TestCase<T, Q>::createArguments() {
  std::vector<T> result;
  for (size_t i = 0; i < _argumentGenerator.size(); ++i) {
    result.emplace_back(_argumentGenerator[i]());
  }
  return result;
}

template <typename T, typename Q>
TestResult TestCase<T, Q>::readCaseData(SyntaxNode &node) {
  if (node.data == "assert" || node.data == "!assert") {
    if (node.data == "assert") {
      return {1, 0, 0, true};
    }
    return {1, 0, 0, false};
  }
  return {(size_t)std::atoi(node.data.c_str()), 0, 0, true};
}

template <typename T, typename Q>
std::function<T()> TestCase<T, Q>::createRandom(const std::string &size) {
  size_t argSize = std::atoi(size.c_str());
  return [argSize]() {
    Q &io = Q::getInstance();
    return T(io.createRandom(argSize));
  };
}

template <typename T, typename Q>
std::function<T()> TestCase<T, Q>::createUnsigned(const std::string &size) {
  size_t argSize = std::atoi(size.c_str());
  return [argSize]() {
    Q &io = Q::getInstance();
    return T(io.createRandom(argSize, false));
  };
}

template <typename T, typename Q>
std::function<T()> TestCase<T, Q>::createSigned(const std::string &size) {
  size_t argSize = std::atoi(size.c_str());
  return [argSize]() {
    Q &io = Q::getInstance();
    return T(io.createRandom(argSize, true));
  };
}

template <typename T, typename Q>
std::function<T()> TestCase<T, Q>::createRange(const std::string &min,
                                               const std::string &max) {
  size_t minArg = std::atoi(min.c_str());
  size_t maxArg = std::atoi(max.c_str());
  return [minArg, maxArg]() {
    Q &io = Q::getInstance();
    return T(io.createRandom(minArg, maxArg));
  };
}

template <typename T, typename Q>
std::function<T()> TestCase<T, Q>::createValue(const std::string &source) {
  T argSource(source);
  return [argSource]() { return argSource; };
}

#endif /*TESTCASE*/
