#ifndef SYNTAXNODE
#define SYNTAXNODE

#include <string>
#include <vector>

class SyntaxNode {
public:
  std::string data;
  std::vector<SyntaxNode> children;
};

#endif /*SYNTAXNODE*/
