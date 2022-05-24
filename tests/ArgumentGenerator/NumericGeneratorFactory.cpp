#include "NumericGeneratorFactory.h"
#include "../../tester/syntaxnode.h"
#include "RandomParameter.h"
#include "RangedParameter.h"
#include "ValuedParameter.h"

NumericGeneratorFactory::NumericGeneratorFactory(KCrypt::BasicIo &io,
                                                 KCrypt::AddEngine &adder)
    : _io(io), _adder(adder) {}

ArgumentGenerator<KCrypt::Numeric> *
NumericGeneratorFactory::create(SyntaxNode &node) {
  if (node.data == "random" || node.data == "signed" ||
      node.data == "unsigned") {
    return new RandomParameter(_io, std::atoi(node.children[0].data.c_str()),
                               node.data);
  } else if (node.data == "range") {
    return new RangedParameter(_io, std::atoi(node.children[0].data.c_str()),
                               std::atoi(node.children[1].data.c_str()));
  } else if (node.data == "value") {
    return new ValuedParameter(_io, _adder, node.children[0].data);
  } else {
    throw std::runtime_error("Invalid parameter type: " + node.data);
  }
}
