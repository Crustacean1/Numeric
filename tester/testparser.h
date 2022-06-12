#ifndef TESTPARSER
#define TESTPARSER

#include <functional>
#include <regex>

#include "syntaxnode.h"

class TestParser {

  static constexpr const char *__operandExpression = "([a-z]*)\\(([0-9]*)\\)";
  static constexpr const char *__caseExpresssion = "(([a-z]|[A-Z])*)a:";
  static constexpr const char *__testExpression = "";

  using ParsingStage = void (TestParser::*)(SyntaxNode &, std::string);

  std::vector<SyntaxNode> parseLowerStage(std::string &source,
                                          const std::string &pattern,
                                          int dataPos, int bodyPos,
                                          ParsingStage func);

  SyntaxNode iterateNode(std::string &data, ParsingStage func);

  std::string loadFromStream(std::istream &file);

  void parseFile(SyntaxNode &node, std::string data);
  void parseTest(SyntaxNode &node, std::string data);
  void parseCase(SyntaxNode &node, std::string data);
  void parseOperand(SyntaxNode &node, std::string data);
  void parseLiteral(SyntaxNode &node, std::string data);

  std::string readTestStream(std::istream &stream);

public:
  TestParser();
  SyntaxNode parseStream(std::istream &stream);
};

#endif /*TESTPARSER*/
