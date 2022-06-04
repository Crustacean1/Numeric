#include "ExpEngine.h"
#include "../BasicIo.h"
#include "AddEngine.h"
#include "CompEngine.h"
#include "DivEngine.h"
#include "MulEngine.h"
#include <iostream>

using namespace KCrypt;

ExpEngine::ExpEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul,
                     DivEngine &div, Buffer &buffer3, Buffer &buffer4)
    : _cmp(cmp), _add(add), _mul(mul), _div(div), _buffer4(buffer4),
      _buffer3(buffer3) {}

void ExpEngine::fastModExp(const BufferView &base, const BufferView &exponent,
                           const BufferView &modulus,
                           const BufferView &modInverse, size_t binPoint,
                           const BufferView &result) {

  reserveModExpBuffers(modulus.size);

  BufferView value(_buffer3.splice(modulus.size));

  result.clear();
  result.data[0] = 1;

  size_t currentDigit;

  for (int i = _cmp.topOne(exponent); i >= 0; --i) {
    currentDigit = exponent.data[i / BufferView::WordSize];
    currentDigit >>= (i % BufferView::WordSize);

    _mul.kar(result, result, value);
    _div.fastModulo(value, modulus, modInverse, result, binPoint);

    if (currentDigit & 1) {
      _mul.kar(result, base, value);
      _div.fastModulo(value, modulus, modInverse, result, binPoint);
    }
  }
}

void ExpEngine::reserveModExpBuffers(size_t modulusSize) {
  _buffer3.reserve(modulusSize * 2);
  _buffer4.reserve(modulusSize * 2);
}
