#include "ExpEngine.h"
#include "../ArithmInjector.h"
#include "AddEngine.h"
#include "CompEngine.h"
#include "DivEngine.h"
#include "IoEngine.h"
#include "MulEngine.h"
#include <iostream>

using namespace KCrypt;

ExpEngine::ExpEngine(ArithmInjector & injector)
    : _cmp(injector.getCmp()),
      _add(injector.getAdd()),
      _mul(injector.getMul()),
      _io(injector.getIo()),
      _div(injector.getDiv()), _buffer(1),
      _value(_buffer) {}

void ExpEngine::fastModExp(const BufferView &base, const BufferView &exponent,
                           const BufferView &modulus,
                           const BufferView &modInverse, size_t binPoint,
                           const BufferView &result) {

  reserveModExpBuffers(modulus.size);

  result.clear();
  result.data[0] = 1;

  size_t currentDigit;

  for (int i = _cmp.topOne(exponent); i >= 0; --i) {
    currentDigit = exponent.data[i / BufferView::WordSize];
    currentDigit >>= (i % BufferView::WordSize);

    _mul.kar(result, result, _value);
    _div.fastModulo(_value, modulus, modInverse, result, binPoint);

    if (currentDigit & 1) {
      _mul.kar(result, base, _value);
      _div.fastModulo(_value, modulus, modInverse, result, binPoint);
    }
  }
}

void ExpEngine::reserveModExpBuffers(size_t modulusSize) {
  _buffer.reserve(modulusSize * 2);
  _value = _buffer.splice(modulusSize * 2);
}
