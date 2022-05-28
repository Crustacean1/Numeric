#include "DivEngine.h"
#include "AddEngine.h"
#include "CompEngine.h"
#include "MulEngine.h"

using namespace KCrypt;

DivEngine::DivEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul,
                     IntBuffer &buff1, IntBuffer &buff2)
    : _cmp(cmp), _add(add), _mul(mul), _aDivBuffer(buff1), _bDivBuffer(buff2) {}

void DivEngine::div(const IntBufferView &a, const IntBufferView &b,
                    const IntBufferView &c) {
  size_t aShift = a.size * wordSize - _cmp.leftOffset(a);
  size_t bShift = b.size * wordSize - _cmp.leftOffset(b);
  size_t shift = aShift - bShift;

  if (shift >> (sizeof(size_t) * 8 - 1)) {
    return;
  }

  _aDivBuffer.reserve(a.size);
  _bDivBuffer.reserve(a.size);
  IntBufferView aBuf = _aDivBuffer.splice(0, a.size);
  IntBufferView bBuf = _bDivBuffer.splice(0, a.size);

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

  size_t shiftOffset = wordSize - ((shift + 1) % wordSize);
  size_t lastPos = (shift + 1) / wordSize;

  c.data[lastPos] <<= shiftOffset;
  c.data[lastPos] >>= shiftOffset;
  c.data[lastPos] *= (shiftOffset != 32);
}

void DivEngine::newtonDiv(const IntBufferView &dividend,
                          const IntBufferView &inverse,
                          const IntBufferView &output, size_t invPrecision) {

  _aDivBuffer.reserve(inverse.size << 1);

  _aDivBuffer.clear();

  _mul.kar(inverse, dividend, _aDivBuffer);

  _add.rightShift(_aDivBuffer, _aDivBuffer, invPrecision);
  output.copy(_aDivBuffer.splice(0, dividend.size));
}

size_t DivEngine::divApprox(const IntBufferView &divisor,
                            const IntBufferView &inverse) {

  size_t aSigPos = _cmp.topOne(divisor);
  size_t aWordPos = aSigPos / wordSize;
  size_t aSigShift = aSigPos % wordSize;

  BufferInt approx = divisor.data[aWordPos];
  approx <<= wordSize;
  approx += (aWordPos == 0) * divisor.data[(aWordPos != 0) * (aWordPos - 1)];
  approx >>= (aSigShift + 1);

  _buffer.major = 1;
  _buffer.major <<= highBufferPos;
  _buffer.major /= approx;

  inverse.data[inverse.size - 1] = _buffer.major;
  return aSigPos + inverse.size * wordSize;
}

size_t DivEngine::newtonInverse(const IntBufferView &divisor,
                                const IntBufferView &inverse) {

  _aDivBuffer.reserve((inverse.size * 2));
  _bDivBuffer.reserve((inverse.size * 4));

  size_t invPrecision = divApprox(divisor, inverse);

  size_t digitsOfAccuracy = 1;
  for (; digitsOfAccuracy != inverse.size; digitsOfAccuracy <<= 1) {
    newtonIteration(divisor, inverse, invPrecision);
  }
  newtonIteration(divisor, inverse, invPrecision);
  _add.add(inverse, 1);
  return invPrecision;
}

void DivEngine::newtonIteration(const IntBufferView &divisor,
                                const IntBufferView &inverse,
                                size_t precision) {

  size_t wordPos = (precision + 1) / wordSize;
  size_t bitPos = (precision + 1) - wordPos * wordSize;

  const IntBufferView &innerProductBuffer =
      _aDivBuffer.splice(0, inverse.size * 2);
  const IntBufferView &outerProductBuffer =
      _bDivBuffer.splice(0, innerProductBuffer.size * 2);

  const IntBufferView &hBuff =
      _aDivBuffer.splice(wordPos, _aDivBuffer.size - wordPos);

  _mul.kar(inverse, divisor, innerProductBuffer);

  _add.invert(innerProductBuffer);
  _add.add(hBuff, (BaseInt(1) << bitPos));

  outerProductBuffer.clear();

  _mul.kar(innerProductBuffer, inverse, outerProductBuffer);

  _add.rightShift(outerProductBuffer, outerProductBuffer, precision);
  inverse.copy(outerProductBuffer);
}
