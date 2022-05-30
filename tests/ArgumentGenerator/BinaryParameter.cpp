#include "BinaryParameter.h"
#include "../../numeric/BasicIo.h"
#include "../../numeric/Numeric.h"
#include <stdexcept>

using namespace KCrypt;

BinaryParameter::BinaryParameter(std::string source, AddEngine &add)
    : _source(source), _add(add) {}

Numeric BinaryParameter::createInstance(std::default_random_engine &e) {
  Buffer buffer(_source.size() / BufferView::WordSize);
  buffer.splice().clear();

  BufferView view = buffer.splice();

  for (const auto binDigit : _source) {
    if (binDigit != '1' && binDigit != '0') {
      throw std::runtime_error(
          std::string(
              "Invalid character in string passed to binary operand: ") +
          _source);
    }
    _add.leftShift(view, view, 1);
    _add.add(view, (binDigit == '1'));
  }
  return Numeric(std::move(buffer));
}

BinaryParameter::~BinaryParameter() {}
