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

  std::cout<<"Base: "<<base.size<<" modulus: "<<modulus.size<<" inverse: "<<inverse.size<<std::endl;
  size_t modPrecision = _div.newtonInverse(modulus, inverse);
  std::cout<<"inverse: "<<io.toDecimal(inverse)<<std::endl;

  result.clear();
  result.data[0] = 1;

  std::cout << "Starting from : " << io.toDecimal(result) << std::endl;

  for (int i = _cmp.topOne(exponent); i > 0; --i) {
    bool one =
        (exponent.data[i / BufferView::WordSize] >> (i % BufferView::WordSize));
    if (one) {
      _mul.kar(result, base, value);
      std::cout << "Mul Pre " << io.toDecimal(result) << std::endl;
      fastModulo(value, modulus, inverse, result, modPrecision);
      std::cout << "Mul Post " << io.toDecimal(result) << std::endl;
    }
    _mul.kar(result, result, value);
    std::cout << "Pre " << io.toDecimal(result) << std::endl;
    fastModulo(value, modulus, inverse, result, modPrecision);
    std::cout << "Post " << io.toDecimal(result) << std::endl;
  }
}

void ExpEngine::fastModulo(const BufferView &arg, const BufferView &modulus,
                           const BufferView &modInverse,
                           const BufferView &result, size_t precision) {
  BasicIo io(_cmp, _add);
  _div.newtonDiv(arg, modInverse, _modBuffer.splice(modulus.size), precision);

  std::cout << "div result: " << io.toDecimal(arg) << " / "
            << io.toDecimal(modulus) << " = "
            << io.toDecimal(_modBuffer.splice(modulus.size)) << std::endl;

  _mul.kar(_modBuffer, modulus, _subBuffer.splice(modulus.size * 2));
  _add.subFromLeft(arg, _subBuffer);

  std::cout << "subtracting: " << io.toDecimal(_subBuffer) << std::endl;

  result.copy(arg);
}
