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
    : _cmp(cmp), _add(add), _mul(mul), _div(div), _modBuffer(buffer1),
      _invBuffer(buffer2), _expBuffer(buffer3), _subBuffer(buffer4) {}

void ExpEngine::reserveBuffers(size_t modulusSize) {
  _modBuffer.reserve(modulusSize);
  _subBuffer.reserve(modulusSize * 2);
  _expBuffer.reserve(modulusSize * 2);
  _invBuffer.reserve(modulusSize * 2);
}
void ExpEngine::modExp(const BufferView &base, const BufferView &exponent,
                       const BufferView &modulus, const BufferView &result) {

  BasicIo io(_cmp, _add);
  reserveBuffers(modulus.size);

  BufferView value(_expBuffer.splice(modulus.size));
  BufferView inverse(_invBuffer.splice(modulus.size * 2));

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
  BasicIo io(_cmp, _add);
  _div.newtonDiv(arg, modInverse, _modBuffer.splice(modulus.size), precision);

  _mul.kar(_modBuffer, modulus, _subBuffer.splice(modulus.size * 2));
  _add.subFromLeft(arg, _subBuffer);

  result.copy(arg);
}
