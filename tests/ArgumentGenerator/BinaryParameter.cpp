#include "BinaryParameter.h"
#include "../../numeric/BasicIo.h"
#include "../../numeric/Numeric.h"

using namespace KCrypt;

BinaryParameter::BinaryParameter(std::string source, AddEngine &add)
    : _source(source), _add(add) {}

Numeric BinaryParameter::createInstance(std::default_random_engine &e) {
  IntBuffer buffer(_source.size() / wordSize);
  buffer.clear();

  for (const auto binDigit : _source) {
    if (binDigit != '1' && binDigit != '0') {
      throw std::runtime_error(
          std::string(
              "Invalid character in string passed to binary operand: ") +
          _source);
    }
    _add.leftShift(buffer, buffer, 1);
    _add.add(buffer, (binDigit == '1'));
  }
  return Numeric(buffer);
}

BinaryParameter::~BinaryParameter() {}
