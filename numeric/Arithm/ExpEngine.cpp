#include "ExpEngine.h"
#include "../BasicIo.h"
#include "AddEngine.h"
#include "CompEngine.h"
#include "DivEngine.h"
#include "MulEngine.h"
#include <iostream>

using namespace KCrypt;

ExpEngine::ExpEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul,
                     DivEngine &div, Buffer &buffer1, Buffer &buffer2,
                     Buffer &buffer3, Buffer &buffer4)
    : _cmp(cmp), _add(add), _mul(mul), _div(div), _buffer1(buffer1),
      _buffer4(buffer2), _buffer3(buffer3), _buffer2(buffer4) {}

void ExpEngine::modExp(const BufferView &base, const BufferView &exponent,
                       const BufferView &modulus, const BufferView &result) {

  reserveModExpBuffers(modulus.size);

  BufferView value(_buffer3.splice(modulus.size));
  BufferView inverse(_buffer4.splice(modulus.size * 2));

  size_t modPrecision = _div.newtonInverse(modulus, inverse);

  result.clear();
  result.data[0] = 1;

  size_t currentDigit;

  for (int i = _cmp.topOne(exponent); i >= 0; --i) {
    currentDigit = exponent.data[i / BufferView::WordSize];
    currentDigit >>= (i % BufferView::WordSize);

    _mul.kar(result, result, value);
    fastModulo(value, modulus, inverse, result, modPrecision);

    if (currentDigit & 1) {
      _mul.kar(result, base, value);
      fastModulo(value, modulus, inverse, result, modPrecision);
    }
  }
}

void ExpEngine::fastModulo(const BufferView &arg, const BufferView &modulus,
                           const BufferView &modInverse,
                           const BufferView &result, size_t precision) {
  _div.newtonDiv(arg, modInverse, _buffer1.splice(modulus.size), precision);

  _mul.kar(_buffer1, modulus, _buffer2.splice(modulus.size * 2));
  _add.subFromLeft(arg, _buffer2);

  // Temporary assert
  if (_cmp.greaterOrEqual(arg, modulus)) {
    BasicIo io(_cmp, _add);
    std::cout << "Critical Error, invalid modulo operation for:" << std::endl;
    std::cout << "Mod: " << io.toDecimal(modulus) << std::endl;
    std::cout << "Arg: " << io.toDecimal(arg) << std::endl;
  }

  result.copy(arg);
}

void ExpEngine::gcdDebug(GcdExtension &ext) {
  BasicIo io(_cmp, _add);
  std::cout << "Ext: " << io.toDecimal(ext.value)
            << "\nA: " << io.toDecimal(ext.coefficientA)
            << "\nB: " << io.toDecimal(ext.coefficientB) << std::endl;
}

void ExpEngine::extendedGcd(const BufferView &a, const BufferView &b,
                            const BufferView &aCoeff,
                            const BufferView &bCoeff) {

  BasicIo io(_cmp, _add);

  std::cout << "Gcd start: " << std::endl;

  reserveExtGcdBuffers(a.size, b.size);

  GcdExtension gcdA{aCoeff, _buffer1.splice(bCoeff.size),
                    _buffer2.splice(bCoeff.size)};

  GcdExtension gcdB{bCoeff, _buffer3.splice(aCoeff.size),
                    _buffer4.splice(aCoeff.size)};

  init(gcdA, a, 0);
  init(gcdB, b, 1);

  while (!_cmp.empty(gcdA.value) && !_cmp.empty(gcdB.value)) {

    bool isAReducible = isReducible(gcdA);
    bool isBReducible = isReducible(gcdB);
    if (isAReducible && isBReducible) {
      reduceBoth(gcdA, gcdB);
    } else if (isAReducible) {
      reduceLeft(gcdA, gcdB);
    } else if (isBReducible) {
      reduceLeft(gcdB, gcdA);
    } else {
      swap(gcdA, gcdB);
    }
    gcdDebug(gcdA);
    gcdDebug(gcdB);
    std::cout << "------------------------------\n";
  }

  if (_cmp.empty(gcdA.value)) {
    aCoeff.copy(gcdB.coefficientA);
    bCoeff.copy(gcdB.coefficientB);
    return;
  }
  aCoeff.copy(gcdA.coefficientA);
  bCoeff.copy(gcdA.coefficientB);
}

void ExpEngine::init(GcdExtension &ext, const BufferView &view, bool position) {
  ext.coefficientA.clear();
  ext.coefficientB.clear();

  ext.value.copy(view);

  if (position) {
    ext.coefficientB.data[0] = 1;
  } else {
    ext.coefficientA.data[0] = 1;
  }
}

bool ExpEngine::isReducible(GcdExtension &ext) {
  return (ext.value.data[0] & 1) == 0;
}

void ExpEngine::halve(GcdExtension &a, GcdExtension &b) {
  _add.rightShift(a.value, a.value, 1);
  if ((a.coefficientA.data[0] & 1) == 0 && (a.coefficientB.data[0] & 1) == 0) {
    _add.rightShift(a.coefficientA, a.coefficientA, 1);
    _add.rightShift(a.coefficientB, a.coefficientB, 1);
  }
  _add.leftShift(b.coefficientA, b.coefficientA, 1);
  _add.leftShift(b.coefficientB, b.coefficientB, 1);
}

void ExpEngine::reduce(GcdExtension &ext) {
  size_t offset = _cmp.rightOffset(ext.value);
  _add.rightShift(ext.value, ext.value, offset);
}

void ExpEngine::subtract(GcdExtension &a, GcdExtension &b) {
  _add.subFromLeft(a.value, b.value);
  _add.subFromLeft(a.coefficientA, b.coefficientA);
  _add.subFromLeft(a.coefficientB, b.coefficientB);
}

void ExpEngine::reserveExtGcdBuffers(size_t aSize, size_t bSize) {
  _buffer1.reserve(aSize);
  _buffer2.reserve(bSize);
  _buffer3.reserve(aSize);
  _buffer4.reserve(bSize);
}

void ExpEngine::reserveModExpBuffers(size_t modulusSize) {
  _buffer1.reserve(modulusSize);
  _buffer2.reserve(modulusSize * 2);
  _buffer3.reserve(modulusSize * 2);
  _buffer4.reserve(modulusSize * 2);
}
