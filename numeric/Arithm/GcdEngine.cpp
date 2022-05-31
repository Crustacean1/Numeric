#include "GcdEngine.h"
#include "../BasicIo.h"
#include "AddEngine.h"
#include "CompEngine.h"

#include <iostream>

using namespace KCrypt;

GcdEngine::GcdEngine(CompEngine &cmp, AddEngine &add, Buffer &a1, Buffer &a2,
                     Buffer &b1, Buffer &b2, Buffer &c, Buffer &d)
    : _cmp(cmp), _add(add), _a1(a1), _b1(b1), _a2(a2), _b2(b2), _c(c), _d(d) {}

void GcdEngine::gcdDebug(GcdExtension &ext) {
  IoEngine io(_cmp, _add);
  std::cout << "Ext: " << io.toDecimal(ext.value)
            << "\nA: " << io.toDecimal(ext.a) << "\nB: " << io.toDecimal(ext.b)
            << std::endl;
}

void GcdEngine::extendedGcd(const BufferView &a, const BufferView &b,
                            const BufferView &aCoeff,
                            const BufferView &bCoeff) {

  IoEngine io(_cmp, _add);

  std::cout << "Gcd start: " << std::endl;

  reserveBuffers(a.size, b.size);

  GcdExtension gcdA{aCoeff, _a1.splice(bCoeff.size), _b1.splice(bCoeff.size)};
  GcdExtension gcdB{bCoeff, _a2.splice(aCoeff.size), _b2.splice(aCoeff.size)};

  init(gcdA, a);
  init(gcdB, b);

  BufferView c = _c.splice(a.size);
  BufferView d = _d.splice(b.size);

  bool cmpResult = _cmp.greater(gcdA.value, gcdB.value);

  while (iterate((cmpResult ? gcdA : gcdB), (cmpResult ? gcdB : gcdA), c, d)) {
    cmpResult = _cmp.greater(gcdA.value, gcdB.value);
  }
}

bool GcdEngine::iterate(GcdExtension &ext1, GcdExtension &ext2,
                        BufferView &src1, BufferView &src2) {
  subtract(ext1, ext2);
  while (isEven(ext1.value)) {
    halve(ext1, src1, src2);
  }
  return _cmp.empty(ext1.value);
}

void GcdEngine::init(GcdExtension &ext, const BufferView &view) {
  ext.a.clear();
  ext.b.clear();

  ext.value.copy(view);
  size_t offset = _cmp.rightOffset(ext.value);
  _add.rightShift(ext.value, ext.value, offset);
}

bool GcdEngine::isEven(const BufferView &ext) { return (ext.data[0] & 1) == 0; }

void GcdEngine::halve(GcdExtension &ext, const BufferView &src1,
                      const BufferView &src2) {

  _add.rightShift(ext.value, ext.value, 1);
  if ((ext.a.data[0] & 1) == 0 && (ext.b.data[0] & 1) == 0) {
    _add.rightShift(ext.a, ext.a, 1);
    _add.rightShift(ext.b, ext.b, 1);
    return;
  }
  if (_cmp.isSigned(ext.a)) {
    _add.addToLeft(ext.a, src1);
    _add.subFromLeft(ext.b, src2);
  } else {
    _add.subFromLeft(ext.a, src1);
    _add.addToLeft(ext.b, src2);
  }
}

void GcdEngine::subtract(GcdExtension &a, GcdExtension &b) {
  _add.subFromLeft(a.value, b.value);
  _add.subFromLeft(a.a, b.a);
  _add.subFromLeft(a.b, b.b);
}

void GcdEngine::reserveBuffers(size_t aSize, size_t bSize) {
  _a1.reserve(aSize);
  _a2.reserve(bSize);

  _b1.reserve(aSize);
  _b2.reserve(bSize);

  _c.reserve(aSize);
  _d.reserve(aSize);
}
