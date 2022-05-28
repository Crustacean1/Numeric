#include "NumericGeneratorFactory.h"
#include "../../tester/syntaxnode.h"
#include "BinaryParameter.h"
#include "RandomParameter.h"
#include "RangedParameter.h"
#include "ValuedParameter.h"

using namespace KCrypt;

NumericGeneratorFactory::NumericGeneratorFactory(BasicIo &io, AddEngine &adder)
    : _io(io), _add(adder) {}

ArgumentGenerator<Numeric> *NumericGeneratorFactory::create(SyntaxNode &node) {
  if (node.data == "random" || node.data == "signed" ||
      node.data == "unsigned") {
    return new RandomParameter(_io, std::atoi(node.children[0].data.c_str()),
                               node.data);
  } else if (node.data == "range") {
    return new RangedParameter(_io, std::atoi(node.children[0].data.c_str()),
                               std::atoi(node.children[1].data.c_str()));
  } else if (node.data == "value") {
    return new ValuedParameter(_io, _add, node.children[0].data);
  } else if (node.data == "binary") {
    return new BinaryParameter(node.children[0].data, _add);
  } else {
    throw std::runtime_error("Invalid parameter type: " + node.data);
  }
}
