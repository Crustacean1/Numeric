#ifndef TESTPARSER
#define TESTPARSER

#include "logger.h"
#include "tester.h"

#include <iostream>
#include <random>
#include <regex>

class DataNode {
public:
  std::string data;
  std::vector<DataNode> children;
};

class SyntaxNode {
public:
  std::string data;
  std::vector<SyntaxNode> children;
};

template <typename T> class TestParser {

  std::default_random_engine _engine;
  std::string _currentTest;
  std::vector<T> _arguments;

  using TestArgumentGenerator = std::function<T()>;

  float _totalTime;
  size_t _passed = 0;
  size_t _failed = 0;

  Tester<T> &_tester;
  Logger _logger;

  template <typename... Q> void logInfo(Q... q);

  template <typename... Q> void logError(Q... q);

  void logTestResults();

  void resetState();
  using ParsingStage = void (*)(SyntaxNode &, std::string &);

  static std::vector<SyntaxNode> parseLowerStage(std::string &source,
                                                 const std::string &pattern,
                                                 ParsingStage func);

  static SyntaxNode iterateNode(std::string &data, ParsingStage func);

  static void parseTests(SyntaxNode &node, std::string data);
  static void parseCases(SyntaxNode &node, std::string data);
  static void parseParams(SyntaxNode &node, std::string data);

  std::vector<TestArgumentGenerator> parseArguments(SyntaxNode argNode);

  void executeCase(size_t repetitionCount,
                   std::vector<TestArgumentGenerator> &generator,
                   std::vector<std::string> flags);

  void assess(float result, std::vector<T> &arguments);

  std::string readTestStream(std::istream &stream);

public:
  TestParser(Tester<T> &tester, Logger logger);
  void executeTests(std::istream &stream);
};

template <typename T> void TestParser<T>::executeTests(std::istream &stream) {
  size_t length;
  char *data;
  stream.seekg(0, stream.end);
  length = stream.tellg();
  stream.seekg(0, stream.beg);
  data = new char[length + 1];
  stream.read(data, length);
  data[length] = 0;

  // parseTests(data);
}

template <typename T>
void TestParser<T>::parseTests(SyntaxNode &node, std::string buffer) {

  // remove to higher function
  std::regex newlines("\\s");
  std::string rawData = std::regex_replace(buffer, newlines, "");

  std::string testNameRegex = "[a-z|A-Z]+";
  std::string testBodyRegex = "[a-z|A-Z|\\-|!|0-9|\\s|;|:|,|\\(|\\)]+";
  std::string testRegex("(" + testNameRegex + "\\{(" + testBodyRegex + ")\\})");

  node.children = parseLowerStage(buffer, testRegex, parseCases);
}

template <typename T>
void TestParser<T>::parseCases(SyntaxNode &node, std::string buffer) {

  std::string operands = "(unsigned|signed|random|range|value)";
  std::string argument = operands + "\\([0-9]+|,\\)";
  std::string testCase("(case\\([0-9]+\\):(" + argument + ")+);");

  node.children = parseLowerStage(buffer, testCase, parseParams);
}

template <typename T>
std::vector<SyntaxNode> TestParser<T>::parseLowerStage(std::string &data,
                                                 const std::string &regexStr,
                                                 ParsingStage stageParser) {
  std::vector<SyntaxNode> nodes;
  std::regex reg(regexStr);
  using RegexIterator = std::regex_token_iterator<std::string::iterator>;
  auto testIterator = RegexIterator{data.begin(), data.end(), reg, {1, 2}};
  for (; testIterator != RegexIterator{};) {
    SyntaxNode node{*testIterator++};
    node.children = stageParser(node, *testIterator);
    nodes.push_back(node);
  }
  return nodes;
}

template <typename T>
TestParser<T>::TestParser(Tester<T> &tester, Logger logger)
    : _engine(2137), _tester(tester), _logger(logger) {}

/*


template <typename T> void TestParser<T>::executeTests(std::istream &stream) {
  std::string buffer;
  stream >> buffer;
  while (stream) {
    parseCommand(buffer, stream);
    stream >> buffer;
  }
  logTestResults();
}

template <typename T>
void TestParser<T>::parseCommand(const std::string &command,
                                 std::istream &stream) {
  if (command == "test") {
    logTestResults();
    resetState();

    startNewTest(stream);
    _logger.logInfo("Starting new test: <", _currentTest, ">");
  } else if (command == "case") {
    _logger.logInfo("Executing case test:\n");
    executeLiteral(stream);
  } else if (command == "rand") {
    _logger.logInfo("Executing randomized test:\n");
    executeRandom(stream);
  } else {
    throw std::runtime_error("Invalid syntax, keyword: '" + command +
                             "' not recognized");
  }
}

template <typename T> void TestParser<T>::startNewTest(std::istream &stream) {
  stream >> _currentTest;
}

template <typename T> void TestParser<T>::executeLiteral(std::istream &stream)
{ auto size = _tester.getArgumentCount(_currentTest); std::vector<T>
arguments; std::string buffer; while (size--) { stream >> buffer; T
arg(buffer); arguments.push_back(std::move(arg));
  }
  int expected;
  stream >> expected;
  float testResult = _tester.assert(_currentTest, arguments, expected == 1);
  assess(testResult, arguments);
}

template <typename T> void TestParser<T>::executeRandom(std::istream &stream)
{ size_t argCount, argSize; stream >> argCount >> argSize; std::vector<T>
arguments; while (argCount--) { generateRandomArguments(arguments,
_tester.getArgumentCount(_currentTest), argSize); float result =
_tester.assert(_currentTest, arguments, true); assess(result, arguments);
  }
}

template <typename T>
void TestParser<T>::generateRandomArguments(std::vector<T> &args, size_t
count, size_t size) { args.clear(); while (count--) {
    args.push_back(T::createRandom(size));
  }
}

template <typename T>
void TestParser<T>::assess(float result, std::vector<T> &arguments) {
  if (result < 0) {
    ++_failed;
    _logger.logError("\x1B[31mTEST FAILED\x1B[39;49m");
    for (size_t i = 0; i < arguments.size(); ++i) {
      _logger.logError(arguments[i]);
    }
  } else {
    ++_passed;
    _totalTime += result;
  }
}
template <typename T> void TestParser<T>::resetState() {
  _totalTime = 0;
  _passed = _failed = 0;
}
template <typename T> void TestParser<T>::logTestResults() {
  if (_currentTest.size() != 0) {
    std::cout << "SUMMARY FOR TEST: " << _currentTest << std::endl;
    std::cout << "PASSED: " << _passed << std::endl;
    std::cout << "FAILED: " << _failed << std::endl;
    std::cout << "AVG TIME: " << ((_passed == 0) ? 0 : (_totalTime / _passed))
              << " ms" << std::endl;
  }
}*/

#endif /*TESTPARSER*/
