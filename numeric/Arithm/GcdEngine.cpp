#include "GcdEngine.h"
#include "../BasicIo.h"
#include "AddEngine.h"
#include "CompEngine.h"

#include <iostream>

using namespace KCrypt;

GcdEngine::GcdEngine(CompEngine &cmp, AddEngine &add, Buffer &a1, Buffer &a2,
                     Buffer &b1, Buffer &b2, Buffer &c, Buffer &d)
    : _cmp(cmp), _add(add), _a1(a1), _b1(b1), _a2(a2), _b2(b2), _corr1(c),
      _corr2(d), _aCorrection(_corr1), _bCorrection(_corr2) {}

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

  reserveBuffers(a.size, b.size);

  GcdExtension gcdA{aCoeff, _a1.splice(bCoeff.size), _b1.splice(bCoeff.size)};
  GcdExtension gcdB{bCoeff, _a2.splice(aCoeff.size), _b2.splice(aCoeff.size)};

  size_t aOffset = init(gcdA, a);
  size_t bOffset = init(gcdB, b);

  gcdA.a.data[0] = 1;
  gcdB.b.data[0] = 1;

  initCorrections(gcdA, gcdB);

  bool cmpResult = _cmp.greater(gcdA.value, gcdB.value);

  while (iterate((cmpResult ? gcdA : gcdB), (cmpResult ? gcdB : gcdA))) {
    cmpResult = _cmp.greater(gcdA.value, gcdB.value);
  }
  computeResult((cmpResult ? gcdB : gcdA), aOffset, bOffset, aCoeff, bCoeff);
}

void GcdEngine::computeResult(GcdExtension &ext, size_t offset1, size_t offset2,
                              const BufferView &a, const BufferView &b) {

  reduceOneExtension(ext, ext.a, offset1 - offset2);
  reduceOneExtension(ext, ext.b, offset2 - offset1);

  a.clear();
  b.clear();

  a.copy(ext.a);
  b.copy(ext.b);
}

void GcdEngine::reduceOneExtension(GcdExtension &ext,
                                   const BufferView &extToHalve, int offset) {
  while (offset > 0) {
    --offset;
    if (!isEven(extToHalve)) {
      makeExtensionsEven(ext);
    }
    _add.rightShift(extToHalve, extToHalve, 1);
  }
}

bool GcdEngine::iterate(GcdExtension &ext1, GcdExtension &ext2) {

  subtract(ext1, ext2);

  bool isEmpty = _cmp.empty(ext1.value);
  while (isEven(ext1.value) && !isEmpty) {
    halve(ext1);
    isEmpty = _cmp.empty(ext1.value);
  }
  return !isEmpty;
}

size_t GcdEngine::init(GcdExtension &ext, const BufferView &view) {
  IoEngine io(_cmp, _add);
  ext.a.clear();
  ext.b.clear();

  ext.value.copy(view);
  size_t offset = _cmp.rightOffset(ext.value);
  _add.rightShift(ext.value, ext.value, offset);
  return offset;
}

bool GcdEngine::isEven(const BufferView &ext) { return (ext.data[0] & 1) == 0; }

void GcdEngine::makeExtensionsEven(GcdExtension &ext) {
  if (_cmp.isSigned(ext.a)) {
    _add.addToLeft(ext.a, _bCorrection);
    _add.subFromLeft(ext.b, _aCorrection);
  } else {
    _add.subFromLeft(ext.a, _bCorrection);
    _add.addToLeft(ext.b, _aCorrection);
  }
}

void GcdEngine::halve(GcdExtension &ext) {
  if (!isEven(ext.a) || !isEven(ext.b)) {
    makeExtensionsEven(ext);
  }
  _add.rightShift(ext.value, ext.value, 1);
  _add.rightShift(ext.a, ext.a, 1);
  _add.rightShift(ext.b, ext.b, 1);
}

void GcdEngine::subtract(GcdExtension &a, GcdExtension &b) {
  _add.subFromLeft(a.value, b.value);
  _add.subFromLeft(a.a, b.a);
  _add.subFromLeft(a.b, b.b);
}

void GcdEngine::initCorrections(GcdExtension &a, GcdExtension &b) {
  _corr1.reserve(a.value.size);
  _corr2.reserve(b.value.size);

  _aCorrection = _corr1.splice(a.value.size);
  _bCorrection = _corr2.splice(b.value.size);

  _aCorrection.copy(a.value);
  _bCorrection.copy(b.value);
}

void GcdEngine::reserveBuffers(size_t aSize, size_t bSize) {
  _a1.reserve(aSize);
  _a2.reserve(bSize);

  _b1.reserve(aSize);
  _b2.reserve(bSize);
}
