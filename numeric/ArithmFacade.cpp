#include "ArithmFacade.h"
#include "Arithm/PrimalityEngine.h"
#include <iostream>

using namespace KCrypt;

ArithmFacade::ArithmFacade()
    : _cmp(ArithmInjector::getInstance().getCmp()),
      _add(ArithmInjector::getInstance().getAdd()),
      _mul(ArithmInjector::getInstance().getMul()),
      _div(ArithmInjector::getInstance().getDiv()),
      _exp(ArithmInjector::getInstance().getExp()),
      _gcd(ArithmInjector::getInstance().getGcd()),
      _io(ArithmInjector::getInstance().getIo()),
      _tmpBuffer(ArithmInjector::getInstance().getTmp()) {
}

bool ArithmFacade::isSigned(const BufferView &view) {
  return _cmp.isSigned(view);
}

void ArithmFacade::abs(const BufferView &view) {
  if (_cmp.isSigned(view)) {
    _add.invert(view);
  }
}
void ArithmFacade::invert(const BufferView &view) { _add.invert(view); }

void ArithmFacade::extGcd(const BufferView &arg1, const BufferView &arg2,
                          const BufferView &output1,
                          const BufferView &output2) {
  _gcd.extendedGcd(arg1, arg2, output1, output2);
}

void ArithmFacade::modExp(const BufferView &base, const BufferView &exp,
                          const BufferView &modulus, const BufferView &output) {
  _tmpBuffer.reserve(modulus.size);
  BufferView modInverse = _tmpBuffer.splice(modulus.size);
  size_t decPoint = _div.newtonInverse(modulus, modInverse);
  _exp.fastModExp(base, exp, modulus, modInverse, decPoint, output);
}

void ArithmFacade::divide(const BufferView &dividend, const BufferView &divisor,
                          const BufferView &output) {
  _tmpBuffer.reserve(dividend.size);
  auto inverse = _tmpBuffer.splice(dividend.size);
  size_t decPoint = _div.newtonInverse(divisor, inverse);
  _div.newtonDiv(dividend, inverse, output, decPoint);
}

void ArithmFacade::multiply(const BufferView &factor1,
                            const BufferView &factor2,
                            const BufferView &output) {
  _tmpBuffer.reserve(factor1.size * 2);
  auto result = _tmpBuffer.splice(0, factor1.size * 2);
  _mul.kar(factor1, factor2, result);
  output.copy(result);
}
void ArithmFacade::leftShift(const BufferView &view, size_t shift) {
  _add.leftShift(view, view, shift);
}
void ArithmFacade::rightShift(const BufferView &view, size_t shift) {
  _add.rightShift(view, view, shift);
}

void ArithmFacade::subtract(const BufferView &term,
                            const BufferView &outputTerm) {
  if (term.size < outputTerm.size) {
    _add.subFromLeft(outputTerm, term);
    return;
  }
  _add.subFromRight(term, outputTerm);
}

void ArithmFacade::subtract(BufferView::BaseInt term,
                            const BufferView &outputTerm) {
  _add.sub(outputTerm, term);
}

void ArithmFacade::add(const BufferView &term, const BufferView &outputTerm) {
  if (term.size < outputTerm.size) {
    _add.addToLeft(outputTerm, term);
    return;
  }
  _add.addToRight(term, outputTerm);
}

bool ArithmFacade::equal(const BufferView &a, const BufferView &b) {
  if (a.size > b.size) {
    return _cmp.equal(a, b);
  }
  return _cmp.equal(b, a);
}

bool ArithmFacade::greater(const BufferView &a, const BufferView &b) {
  if (a.size > b.size) {
    return _cmp.greater(a, b);
  }
  return _cmp.lesser(b, a);
}

bool ArithmFacade::lesser(const BufferView &a, const BufferView &b) {
  if (a.size > b.size) {
    return _cmp.lesser(a, b);
  }
  return _cmp.greater(b, a);
}

void ArithmFacade::readFromString(const std::string &str, Buffer &buffer) {
  buffer.reserve(_io.decSizeInBinary(str.size()));
  _io.toComplement(str, buffer);
}

std::string ArithmFacade::writeDecimal(const BufferView &buffer) {
  return _io.toDecimal(buffer);
}
std::string ArithmFacade::writeBinary(const BufferView &buffer) {
  return _io.toBinary(buffer);
}
