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

  gcdA.a.data[0] = 1;
  gcdB.b.data[0] = 1;

  BufferView srcA(_c.splice(a.size));
  BufferView srcB(_d.splice(a.size));

  srcA.copy(gcdA.value);
  srcB.copy(gcdB.value);

  bool cmpResult = _cmp.greater(gcdA.value, gcdB.value);

  std::cout << "Gcd start: " << std::endl;
  std::cout << "A: " << io.toDecimal(gcdA.value) << std::endl;
  std::cout << "B: " << io.toDecimal(gcdB.value) << std::endl;
  std::cout << "gcdA: " << io.toDecimal(gcdA.a) << std::endl;
  std::cout << "gcdB: " << io.toDecimal(gcdA.b) << std::endl;

  while (iterate((cmpResult ? gcdA : gcdB), (cmpResult ? gcdB : gcdA), srcA,
                 srcB)) {
    cmpResult = _cmp.greater(gcdA.value, gcdB.value);
    std::cout << "\n\nGcd Iteration: " << std::endl;
    std::cout << "A: " << io.toDecimal(gcdA.value) << std::endl;
    std::cout << "B: " << io.toDecimal(gcdB.value) << std::endl;
    std::cout << "gcdA: " << io.toDecimal(gcdA.a) << std::endl;
    std::cout << "gcdB: " << io.toDecimal(gcdA.b) << std::endl;
      break;
  }
}

bool GcdEngine::iterate(GcdExtension &ext1, GcdExtension &ext2,
                        BufferView &src1, BufferView &src2) {
  IoEngine io(_cmp, _add);

  subtract(ext1, ext2);

  bool isEmpty = _cmp.empty(ext1.value);
    std::cout<<"\nInnerStart"<<std::endl;
    std::cout<<"A: "<<io.toDecimal(ext1.a)<<std::endl;
    std::cout<<"SrcA: "<<io.toDecimal(src1)<<std::endl;
    std::cout<<"B: "<<io.toDecimal(ext1.b)<<std::endl;
    std::cout<<"SrcB: "<<io.toDecimal(src2)<<std::endl;
  while (isEven(ext1.value) && !isEmpty) {
    halve(ext1, src1, src2);
    isEmpty = _cmp.empty(ext1.value);
    std::cout<<"\nInnerIter"<<std::endl;
    std::cout<<"ValueA: "<<io.toDecimal(ext1.value)<<std::endl;
    std::cout<<"ValueB: "<<io.toDecimal(ext2.value)<<std::endl;
    std::cout<<"A: "<<io.toDecimal(ext1.a)<<std::endl;
    std::cout<<"B: "<<io.toDecimal(ext1.b)<<std::endl;
    std::cout<<"SrcA: "<<io.toDecimal(src1)<<std::endl;
    std::cout<<"SrcB: "<<io.toDecimal(src2)<<std::endl;
  }
  return !isEmpty;
}

void GcdEngine::init(GcdExtension &ext, const BufferView &view) {
  IoEngine io(_cmp,_add);
  ext.a.clear();
  ext.b.clear();

  ext.value.copy(view);
  size_t offset = _cmp.rightOffset(ext.value);
  std::cout<<"Offset of: "<<io.toBinary(ext.value)<<std::endl;
  std::cout<<"Offset: "<<offset<<std::endl;
  _add.rightShift(ext.value, ext.value, offset);
}

bool GcdEngine::isEven(const BufferView &ext) { return (ext.data[0] & 1) == 0; }

void GcdEngine::halve(GcdExtension &ext, const BufferView &src1,
                      const BufferView &src2) {
  IoEngine io(_cmp,_add);

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
  _d.reserve(bSize);
}
