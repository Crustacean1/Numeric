#include "ArithmFacade.h"

using namespace KCrypt;

ArithmFacade::ArithmFacade()
    : _cmp(), _add(_cmp), _mul(_cmp, _add, _buffInst[0]), _io(_cmp, _add),
      _div(_cmp, _add, _mul, _buffInst[1], _buffInst[2]),
      _exp(_cmp, _add, _mul, _div, _buffInst[3], _buffInst[4], _buffInst[5],
           _buffInst[6]),
      _gcd(_cmp, _add, _buffInst[3], _buffInst[4], _buffInst[5], _buffInst[6],
           _buffInst[7], _buffInst[8]) {}

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
  output1.clear();
  output2.clear();
}

void ArithmFacade::modExp(const BufferView &base, const BufferView &exp,
                          const BufferView &modulus, const BufferView &output) {
  _exp.modExp(base, exp, modulus, output);
}
void ArithmFacade::divide(const BufferView &dividend, const BufferView &divisor,
                          const BufferView &output) {
  _buffInst[3].reserve(dividend.size);
  auto inverse = _buffInst[3].splice(dividend.size);
  size_t decPoint = _div.newtonInverse(dividend, inverse);
  _div.newtonDiv(dividend, inverse, output, decPoint);
}
void ArithmFacade::multiply(const BufferView &factor1,
                            const BufferView &factor2,
                            const BufferView &output) {
  _buffInst[1].reserve(factor1.size);
  auto result = _buffInst[1].splice(0, factor1.size * 2);
  _mul.kar(factor1, factor2, result);
  output.copy(result);
}
void ArithmFacade::leftShift(const BufferView &view, size_t shift,
                             const BufferView &output) {
  _add.leftShift(view, view, shift);
}
void ArithmFacade::rightShift(const BufferView &view, size_t shift,
                              const BufferView &output) {
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

std::string ArithmFacade::writeToString(const BufferView &buffer){
  return _io.toDecimal(buffer);
}
