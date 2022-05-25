#define __DEBUG(depth, x)                                                      \
  if (true) {                                                                  \
    for (int dpth = 0; dpth < depth; ++dpth) {                                 \
      std::cout << " ";                                                        \
    }                                                                          \
    std::cout << "at: " << __LINE__ << ",\t" << #x << "\t=\t" << x.size        \
              << ": " << io.toDecimal(x, _comp) << " " << _comp.isSigned(x)    \
              << std::endl;                                                    \
  }

#include "MulEngine.h"
#include "../BasicIo.h"
#include "../BufferInstance.h"
#include "../Utils.h"
#include "AddEngine.h"
#include "Comparator.h"
#include <iostream>

namespace KCrypt {

MulEngine::MulEngine(Comparator &comp, AddEngine &add, IntBuffer &kBuffer,
                     IntBuffer &dBuffer1, IntBuffer &dBuffer2)
    : _comp(comp), _adder(add), karBuffer(kBuffer), divBuffer1(dBuffer1),
      divBuffer2(dBuffer2) {}

void MulEngine::leftShift(const IntBuffer &a, const IntBuffer &b,
                          size_t shift) {
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);
  size_t complShift = wordSize - minorShift;

  size_t i = a.size - 1;
  size_t j = a.size - majorShift;
  _buffer.major = a.data[j - 1];
  _buffer.major <<= wordSize;
  for (j = j - 1; j > 0; --i, --j) {
    _buffer.major += a.data[j - 1];
    b.data[i] = (_buffer.major >> complShift);
    _buffer.major <<= wordSize;
  }
  b.data[majorShift] = (a.data[j] << minorShift);
  for (; i > 0; --i) {
    b.data[i - 1] = 0;
  }
}
void MulEngine::rightShift(const IntBuffer &a, const IntBuffer &b,
                           size_t shift) {
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);

  size_t i = 0;
  size_t j = majorShift;
  _buffer.major = a.data[j];

  for (j = j + 1; j < a.size; ++i, ++j) {
    _buffer.minor.high = a.data[j];
    b.data[i] = (_buffer.major >> minorShift);
    _buffer.major >>= wordSize;
  }

  b.data[i++] = (_buffer.minor.low >> minorShift);
  for (; i < b.size; ++i) {
    b.data[i] = 0;
  }
}

void MulEngine::mul(const IntBuffer &b, const IntBuffer &a) {
  BaseInt mulCache;
  for (int i = a.size - 1, j = 0; i >= 0; --i) {
    mulCache = a.data[i];
    _buffer.major = 0;
    a.data[i] = 0;
    for (j = 0; j + i < a.size & j < b.size; ++j) {
      _buffer.major += a.data[i + j];
      _buffer.major += mulCache * b.data[j];
      a.data[i + j] = _buffer.major;
      _buffer.major >>= wordSize;
    }
    for (; j + i < a.size && _buffer.major != 0; ++j) {
      _buffer.major += a.data[i + j];
      a.data[i + j] = _buffer.major;
      _buffer.major >>= wordSize;
    }
  }
}

void MulEngine::div(const IntBuffer &a, const IntBuffer &b,
                    const IntBuffer &c) {
  BasicIo io;
  size_t aShift = a.size * wordSize - _comp.leftOffset(a);
  size_t bShift = b.size * wordSize - _comp.leftOffset(b);
  size_t shift = aShift - bShift;

  if (shift >> (sizeof(size_t) * 8 - 1)) {
    return;
  }

  divBuffer1.reserve(a.size);
  divBuffer2.reserve(a.size);
  IntBuffer aBuf = divBuffer1.splice(0, a.size);
  IntBuffer bBuf = divBuffer2.splice(0, a.size);

  aBuf.copy(a);
  bBuf.copy(b);

  c.clear();

  leftShift(bBuf, bBuf, shift);

  for (size_t i = 0; i < shift + 2; ++i) {
    leftShift(c, c, 1);
    if (_comp.isSigned(aBuf)) {
      _adder.addToLeft(aBuf, bBuf);
    } else {
      _adder.subFromLeft(aBuf, bBuf);
      c.data[0] += 1;
    }
    rightShift(bBuf, bBuf, 1);
  }

  size_t shiftOffset = wordSize - ((shift + 1) % wordSize);
  size_t lastPos = (shift + 1) / wordSize;

  c.data[lastPos] <<= shiftOffset;
  c.data[lastPos] >>= shiftOffset;
  c.data[lastPos] *= (shiftOffset != 32);
}

void MulEngine::kar(const IntBuffer &a, const IntBuffer &b,
                    const IntBuffer &c) {
  karBuffer.reserve(a.size << 2);
  //karBuffer.clear();
  karIt(a, b, c, karBuffer);
}

void MulEngine::karIt(const IntBuffer &a, const IntBuffer &b,
                      const IntBuffer &c, const IntBuffer &d, size_t level) {

  BasicIo io;

  size_t pivot = (a.size >> 1);
  size_t majorPivot = a.size;
  size_t bufferPivot = (a.size << 1);

  if (pivot == 0 || b.size == 0) {
    c.clear();
    if (b.size == 0) {
      return;
    }
    _buffer.major = a.data[0];
    _buffer.major *= b.data[0];
    c.data[0] = _buffer.minor.low;
    c.data[1] = _buffer.minor.high;
    return;
  }

  const IntBuffer la = a.splice(0, pivot);
  const IntBuffer ha = a.splice(pivot, pivot);
  const IntBuffer lb = b.splice(0, K::min(pivot, b.size));
  const IntBuffer hb = b.splice(lb.size, K::min(b.size - lb.size, pivot));

  const IntBuffer lc = c.splice(0, majorPivot);
  const IntBuffer hc = c.splice(majorPivot, majorPivot);

  const IntBuffer lBuffer = d.splice(0, pivot);
  const IntBuffer hBuffer = d.splice(pivot, pivot);

  const IntBuffer xBuffer = d.splice(majorPivot, majorPivot);
  const IntBuffer yBuffer = d.splice(majorPivot, majorPivot + 1);
  const IntBuffer rBuffer = d.splice(bufferPivot, bufferPivot);

  karIt(la, lb, lc, rBuffer, level + 1);
  karIt(ha, hb, hc, rBuffer, level + 1);

  lBuffer.copy(ha);
  _adder.subFromLeft(lBuffer, la);
  bool lSign = !_adder.overflow();

  hBuffer.copy(lb);
  _adder.subFromLeft(hBuffer, hb);
  bool hSign = !_adder.overflow();

  if (hSign) {
    _adder.invert(hBuffer);
  }
  if (lSign) {
    _adder.invert(lBuffer);
  }

  karIt(lBuffer, hBuffer, xBuffer, rBuffer, level + 1);

  yBuffer.data[yBuffer.size - 1] = 0;

  if (lSign != hSign) {
    _adder.invert(yBuffer);
  }

  const IntBuffer c34 = c.splice(pivot, pivot * 3);

  _adder.addUnsignedToLeft(yBuffer, lc);
  _adder.addUnsignedToLeft(yBuffer, hc);
  _adder.addUnsignedToLeft(c34, yBuffer);
}

// Constraints: dividend.size <= inverse.size
void MulEngine::newtonDiv(const IntBuffer &dividend, const IntBuffer &inverse,
                          const IntBuffer &output, size_t invPrecision) {

  divBuffer1.reserve(dividend.size * 2);
  divBuffer1.clear();

  karIt(inverse, dividend, divBuffer1, karBuffer);

  rightShift(divBuffer1, divBuffer1, invPrecision);
  output.copy(divBuffer1.splice(0, dividend.size));
}

size_t MulEngine::divApprox(const IntBuffer &divisor,
                            const IntBuffer &inverse) {

  size_t aSigPos = wordSize * divisor.size - _comp.leftOffset(divisor);
  size_t aWordPos = aSigPos / wordSize;
  size_t aSigShift = aSigPos % wordSize;

  BufferInt approx = divisor.data[aWordPos];
  approx <<= wordSize;
  approx += divisor.data[aWordPos - 1];
  approx >>= (aSigShift + 1);

  _buffer.major = 1;
  _buffer.major <<= highBufferPos;
  _buffer.major /= approx;

  inverse.data[inverse.size - 1] = (_buffer.major >> 1);
  return aSigPos - 1 + (inverse.size * wordSize);
}

size_t MulEngine::newtonInverse(const IntBuffer &divisor,
                                const IntBuffer &inverse) {

  divBuffer1.reserve((inverse.size << 1));
  divBuffer2.reserve((inverse.size << 2));

  size_t invPrecision = divApprox(divisor, inverse);

  // Buffer<BaseInt> approxX = inverse.splice(inverse.size - 2, 2);
  // Buffer<BaseInt> approxDiv = divisor.splice(divisor.size - 2, 2);

  // size_t bufferSize = 2;

  /*for (; bufferSize <= divisor.size; (bufferSize <<= 1)) {
    newtonIteration(approxDiv, approxX, invPrecision);

    approxX = inverse.splice(inverse.size - bufferSize, bufferSize);
    approxDiv = divisor.splice(divisor.size - bufferSize, bufferSize);
  }*/
  //_adder.add(approxX, 1);
  return invPrecision;
}

void MulEngine::newtonIteration(const IntBuffer &divisor,
                                const IntBuffer &inverse, size_t precision) {

  precision += 1;
  size_t wordPos = precision / wordSize;
  size_t bitPos = precision - wordPos * wordSize;

  const IntBuffer hBuff = divBuffer1.splice(wordPos, divBuffer1.size - wordPos);

  kar(inverse, divisor, divBuffer1);

  _adder.invert(divBuffer1);
  _adder.add(hBuff, (BaseInt(1) << bitPos));

  kar(divBuffer1, inverse, divBuffer2);
  rightShift(divBuffer2, divBuffer2, inverse.size * wordSize);
  inverse.copy(divBuffer2);
}
} // namespace KCrypt
