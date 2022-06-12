#ifndef ARGUMENT_GENERATOR_FACTORY
#define ARGUMENT_GENERATOR_FACTORY

#include "ArgumentGenerator.h"

class SyntaxNode;

namespace KCrypt {
class BasicIo;
class Numeric;
} // namespace KCrypt

template <typename TestType> class ArgumentGeneratorFactory {
public:
  virtual ArgumentGenerator<TestType>* create(SyntaxNode &node) = 0;
};

#endif /*ARGUMENT_GENERATOR_FACTORY*/
