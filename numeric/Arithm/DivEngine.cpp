#include "DivEngine.h"
#include "AddEngine.h"
#include "CompEngine.h"
#include "MulEngine.h"

#include "../BasicIo.h"
#include <iostream>

using namespace KCrypt;

DivEngine::DivEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul,
                     Buffer &buff1, Buffer &buff2)
    : _cmp(cmp), _add(add), _mul(mul), _aDivBuffer(buff1), _bDivBuffer(buff2) {}

void DivEngine::div(const BufferView &a, const BufferView &b,
                    const BufferView &c) {
  size_t aShift = a.size * BufferView::WordSize - _cmp.leftOffset(a);
  size_t bShift = b.size * BufferView::WordSize - _cmp.leftOffset(b);
  size_t shift = aShift - bShift;

  if (shift >> (sizeof(size_t) * 8 - 1)) {
    return;
  }

  _aDivBuffer.reserve(a.size);
  _bDivBuffer.reserve(a.size);
  BufferView aBuf = _aDivBuffer.splice(0, a.size);
  BufferView bBuf = _bDivBuffer.splice(0, a.size);

  aBuf.copy(a);
  bBuf.copy(b);

  c.clear();

  _add.leftShift(bBuf, bBuf, shift);

  for (size_t i = 0; i < shift + 2; ++i) {
    _add.leftShift(c, c, 1);
    if (_cmp.isSigned(aBuf)) {
      _add.addToLeft(aBuf, bBuf);
    } else {
      _add.subFromLeft(aBuf, bBuf);
      c.data[0] += 1;
    }
    _add.rightShift(bBuf, bBuf, 1);
  }

  size_t shiftOffset =
      BufferView::WordSize - ((shift + 1) % BufferView::WordSize);
  size_t lastPos = (shift + 1) / BufferView::WordSize;

  c.data[lastPos] <<= shiftOffset;
  c.data[lastPos] >>= shiftOffset;
  c.data[lastPos] *= (shiftOffset != 32);
}

void DivEngine::newtonDiv(const BufferView &dividend, const BufferView &inverse,
                          const BufferView &output, size_t invPrecision) {

  IoEngine io(_cmp, _add);

  _aDivBuffer.reserve(inverse.size << 1);
  const BufferView &buff = _aDivBuffer.splice(inverse.size << 1);

  buff.clear();

  _mul.kar(inverse, dividend, buff);

  _add.rightShift(buff, buff, invPrecision);
  output.copy(buff);
}

size_t DivEngine::divApprox(const BufferView &divisor,
                            const BufferView &inverse) {

  size_t aSigPos = _cmp.topOne(divisor);
  size_t aWordPos = aSigPos / BufferView::WordSize;
  size_t aSigShift = aSigPos % BufferView::WordSize;

  BufferView::BufferInt approx = divisor.data[aWordPos];
  approx <<= BufferView::WordSize;
  approx += (aWordPos != 0) * divisor.data[(aWordPos != 0) * (aWordPos - 1)];
  approx >>= (aSigShift + 1);

  _buffer.major = 1;
  _buffer.major <<= (BufferView::BufferHighBit);
  _buffer.major /= approx;

  size_t precision = aSigPos + inverse.size * BufferView::WordSize;

  // Making sure approximation fits in upper digit of inverse
  //
  bool approxCorrection = _buffer.minor.high != 0;

  precision -= approxCorrection;
  _buffer.major >>= approxCorrection;

  inverse.clear();
  inverse.data[inverse.size - 1] = _buffer.major;

  return precision;
}

size_t DivEngine::newtonInverse(const BufferView &divisor,
                                const BufferView &inverse) {

  IoEngine io(_cmp, _add);

  _aDivBuffer.reserve((inverse.size * 2));
  _bDivBuffer.reserve((inverse.size * 4));

  size_t invPrecision = divApprox(divisor, inverse);

  size_t digitsOfAccuracy = 1;

  for (; digitsOfAccuracy <= inverse.size + 1; digitsOfAccuracy <<= 1) {
    newtonIteration(divisor, inverse, invPrecision);
  }
  _add.add(inverse, 1);

  return invPrecision;
}

void DivEngine::newtonIteration(const BufferView &divisor,
                                const BufferView &inverse, size_t precision) {

  size_t wordPos = (precision + 1) / BufferView::WordSize;
  size_t bitPos = (precision + 1) - wordPos * BufferView::WordSize;

  const BufferView &innerProductBuffer =
      _aDivBuffer.splice(0, inverse.size * 2);
  const BufferView &outerProductBuffer =
      _bDivBuffer.splice(0, innerProductBuffer.size * 2);

  const BufferView &hBuff =
      _aDivBuffer.splice(wordPos, _aDivBuffer.size - wordPos);

  _mul.kar(inverse, divisor, innerProductBuffer);

  _add.invert(innerProductBuffer);
  _add.add(hBuff, (BufferView::BaseInt(1) << bitPos));

  outerProductBuffer.clear();

  _mul.kar(innerProductBuffer, inverse, outerProductBuffer);

  _add.rightShift(outerProductBuffer, outerProductBuffer, precision);
  inverse.copy(outerProductBuffer);
}

void DivEngine::fastModulo(const BufferView &arg, const BufferView &modulus,
                           const BufferView &modInverse,
                           const BufferView &result, size_t precision) {
  _bDivBuffer.reserve(modulus.size);
  _aDivBuffer.reserve(modulus.size * 2);

  BufferView quotient(_bDivBuffer.splice(modulus.size));
  BufferView subtractor(_aDivBuffer.splice(modulus.size * 2));

  newtonDiv(arg, modInverse, quotient, precision);

  _mul.kar(quotient, modulus, subtractor);
  _add.subFromLeft(arg, subtractor);

  // Temporary assert
  if (_cmp.greaterOrEqual(arg, modulus)) {
    IoEngine io(_cmp, _add);
    std::cout << "Critical Error, invalid modulo operation for:" << std::endl;
    std::cout << "Mod: " << io.toDecimal(modulus) << std::endl;
    std::cout << "Arg: " << io.toDecimal(arg) << std::endl;
  }

  result.copy(arg);
}
