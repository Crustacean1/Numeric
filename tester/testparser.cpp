#include "testparser.h"

std::string TestParser::loadFromStream(std::istream &file) {
  size_t length;
  char *data;
  file.seekg(0, file.end);
  length = file.tellg();
  file.seekg(0, file.beg);
  data = new char[length + 1];
  file.read(data, length);
  data[length] = 0;
  auto result = std::string(data);
  delete [] data;
  return result;
}

SyntaxNode TestParser::parseStream(std::istream &stream) {

  SyntaxNode mainNode{"main", {}};
  std::regex whitespace("\\s");
  auto data = loadFromStream(stream);

  auto processedData = std::regex_replace(data, whitespace, "");
  parseFile(mainNode, processedData);
  return mainNode;
}

void TestParser::parseFile(SyntaxNode &node, std::string buffer) {
  std::regex newlines("\\s");
  std::string rawData = std::regex_replace(buffer, newlines, "");

  std::string testNameRegex = "([a-z]|[A-Z])+";
  std::string testBodyRegex = "([a-z]|[A-Z]|[0-9]|-|,|:|!|;|\\(|\\))+";
  std::string testRegex("(" + testNameRegex + ")\\{(" + testBodyRegex + ")\\}");

  node.children =
      parseLowerStage(buffer, testRegex, 1, 3, &TestParser::parseTest);
}

void TestParser::parseTest(SyntaxNode &node, std::string buffer) {
  std::string operands =
      "(((unsigned|signed|random|range|value|binary)\\(([0-9]|,|-)+\\))+)";
  std::string caseExp = "(case\\([0-9]+\\)|assert|!assert)";
  std::string testCase(caseExp + ":" + operands + ";");

  node.children =
      parseLowerStage(buffer, testCase, 1, 2, &TestParser::parseCase);
}

void TestParser::parseCase(SyntaxNode &node, std::string buffer) {
  if (node.data.substr(0, 4) == "case") {
    std::smatch caseCount;
    std::regex_match(node.data, caseCount, std::regex("case\\(([0-9]+)\\)"));
    node.data = caseCount[1];
  }

  std::string operandTypes = "(unsigned|signed|random|range|value|binary)";
  std::string operandSyntax = operandTypes + "\\(((-?[0-9]+,?)+)\\)";
  node.children =
      parseLowerStage(buffer, operandSyntax, 1, 2, &TestParser::parseOperand);
}

void TestParser::parseOperand(SyntaxNode &node, std::string buffer) {
  buffer += ","; // dirty hack for proper array termination
  std::string literalSyntax("(-?[0-9]+),");
  node.children =
      parseLowerStage(buffer, literalSyntax, 1, 1, &TestParser::parseLiteral);
}

void TestParser::parseLiteral(SyntaxNode &node, std::string buffer) {
  node.data = buffer;
}

std::vector<SyntaxNode> TestParser::parseLowerStage(std::string &data,
                                                    const std::string &regexStr,
                                                    int dataPos, int bodyPos,
                                                    ParsingStage stageParser) {
  std::vector<SyntaxNode> nodes;
  std::regex reg(regexStr);
  using RegexIterator = std::regex_token_iterator<std::string::iterator>;
  auto testIterator =
      RegexIterator{data.begin(), data.end(), reg, {dataPos, bodyPos}};

  for (; testIterator != RegexIterator{};) {
    std::string data = *testIterator++;
    std::string body = *testIterator++;
    SyntaxNode node{data, {}};
    std::invoke(stageParser, this, node, body);
    nodes.push_back(node);
  }
  return nodes;
}

TestParser::TestParser() {}
