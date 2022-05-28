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
#include "../Buffer/BufferInstance.h"
#include "../Utils.h"
#include "AddEngine.h"
#include "CompEngine.h"
#include <iostream>

namespace KCrypt {

MulEngine::MulEngine(CompEngine &comp, AddEngine &add, IntBuffer &kBuffer)
    : _comp(comp), _adder(add), karBuffer(kBuffer) {}


void MulEngine::mul(const IntBufferView &b, const IntBufferView &a) {
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


void MulEngine::kar(const IntBufferView &a, const IntBufferView &b,
                    const IntBufferView &c) {
  karBuffer.reserve(a.size << 2);
  c.clear();
  karIt(a, b, c, karBuffer);
}

void MulEngine::karIt(const IntBufferView &a, const IntBufferView &b,
                      const IntBufferView &c, const IntBufferView &d, size_t level) {

  size_t pivot = (a.size >> 1);
  size_t majorPivot = a.size;
  size_t bufferPivot = (a.size << 1);

  if (pivot == 0 || b.size == 0) {
    if (b.size == 0) {
      return;
    }
    _buffer.major = a.data[0];
    _buffer.major *= b.data[0];
    c.data[0] = _buffer.minor.low;
    c.data[1] = _buffer.minor.high;
    return;
  }

  const IntBufferView &la = a.splice(0, pivot);
  const IntBufferView &ha = a.splice(pivot, pivot);
  const IntBufferView &lb = b.splice(0, K::min(pivot, b.size));
  const IntBufferView &hb = b.splice(lb.size, K::min(b.size - lb.size, pivot));

  const IntBufferView &lc = c.splice(0, majorPivot);
  const IntBufferView &hc = c.splice(majorPivot, majorPivot);

  const IntBufferView &lBuffer = d.splice(0, pivot);
  const IntBufferView &hBuffer = d.splice(pivot, pivot);

  const IntBufferView &xBuffer = d.splice(majorPivot, majorPivot);
  const IntBufferView &yBuffer = d.splice(majorPivot, majorPivot + 1);
  const IntBufferView &rBuffer = d.splice(bufferPivot, bufferPivot);

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

  const IntBufferView c34 = c.splice(pivot, pivot * 3);

  _adder.addUnsignedToLeft(yBuffer, lc);
  _adder.addUnsignedToLeft(yBuffer, hc);
  _adder.addUnsignedToLeft(c34, yBuffer);
}

// Constraints: dividend.size <= inverse.size

} // namespace KCrypt
