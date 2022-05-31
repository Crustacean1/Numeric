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
    IoEngine io(_cmp, _add);
    std::cout << "Critical Error, invalid modulo operation for:" << std::endl;
    std::cout << "Mod: " << io.toDecimal(modulus) << std::endl;
    std::cout << "Arg: " << io.toDecimal(arg) << std::endl;
  }

  result.copy(arg);
}

void ExpEngine::reserveModExpBuffers(size_t modulusSize) {
  _buffer1.reserve(modulusSize);
  _buffer2.reserve(modulusSize * 2);
  _buffer3.reserve(modulusSize * 2);
  _buffer4.reserve(modulusSize * 2);
}
