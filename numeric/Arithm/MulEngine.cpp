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
#include "../ArithmFacade.h"
#include "../Buffer/BufferInstance.h"
#include "../Utils.h"
#include "AddEngine.h"
#include "CompEngine.h"
#include <iostream>

namespace KCrypt {

MulEngine::MulEngine(ArithmFacade &arithm)
    : _comp(arithm.getCmp()), _adder(arithm.getAdd()),
      karBuffer(arithm.getBuffer(0)) {}

void MulEngine::mul(const BufferView &b, const BufferView &a) {
  BufferView::BaseInt mulCache;
  for (int i = a.size - 1, j = 0; i >= 0; --i) {
    mulCache = a.data[i];
    _buffer.major = 0;
    a.data[i] = 0;
    for (j = 0; j + i < a.size & j < b.size; ++j) {
      _buffer.major += a.data[i + j];
      _buffer.major += mulCache * b.data[j];
      a.data[i + j] = _buffer.major;
      _buffer.major >>= BufferView::WordSize;
    }
    for (; j + i < a.size && _buffer.major != 0; ++j) {
      _buffer.major += a.data[i + j];
      a.data[i + j] = _buffer.major;
      _buffer.major >>= BufferView::WordSize;
    }
  }
}

void MulEngine::kar(const BufferView &a, const BufferView &b,
                    const BufferView &c) {
  /*c.copy(a);
  mul(b,c);*/
  karBuffer.reserve(a.size << 2);
  c.clear();
  karIt(a, b, c, karBuffer.splice(0, a.size << 2));
}

void MulEngine::karIt(const BufferView &a, const BufferView &b,
                      const BufferView &c, const BufferView &d) {

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

  const BufferView &la = a.splice(0, pivot);
  const BufferView &ha = a.splice(pivot, pivot);
  const BufferView &lb = b.splice(0, K::min(pivot, b.size));
  const BufferView &hb = b.splice(lb.size, K::min(b.size - lb.size, pivot));

  const BufferView &lc = c.splice(0, majorPivot);
  const BufferView &hc = c.splice(majorPivot, majorPivot);

  const BufferView &lBuffer = d.splice(0, pivot);
  const BufferView &hBuffer = d.splice(pivot, pivot);

  const BufferView &xBuffer = d.splice(majorPivot, majorPivot);
  const BufferView &yBuffer = d.splice(majorPivot, majorPivot + 1);
  const BufferView &rBuffer = d.splice(bufferPivot, bufferPivot);

  karIt(la, lb, lc, rBuffer);
  karIt(ha, hb, hc, rBuffer);

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

  karIt(lBuffer, hBuffer, xBuffer, rBuffer);

  yBuffer.data[yBuffer.size - 1] = 0;

  if (lSign != hSign) {
    _adder.invert(yBuffer);
  }

  const BufferView c34 = c.splice(pivot, pivot * 3);

  _adder.addUnsignedToLeft(yBuffer, lc);
  _adder.addUnsignedToLeft(yBuffer, hc);
  _adder.addUnsignedToLeft(c34, yBuffer);
}

// Constraints: dividend.size <= inverse.size

} // namespace KCrypt
