#include "ExpEngine.h"
#include "AddEngine.h"
#include "CompEngine.h"
#include "DivEngine.h"
#include "MulEngine.h"

using namespace KCrypt;

ExpEngine::ExpEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul,
                     DivEngine &div, Buffer &modBuffer1, Buffer &modBuffer2)
    : _cmp(cmp), _add(add), _mul(mul), _div(div), _modBuffer(modBuffer1),
      _invBuffer(modBuffer2) {}

void ExpEngine::modExp(const BufferView &base, const BufferView &exponent,
                       const BufferView &modulus, const BufferView &result) {
  _invBuffer.reserve(modulus.size);
  BufferView inverse(_invBuffer.splice(modulus.size));

  _modBuffer.reserve(modulus.size * 2);
  BufferView value(_modBuffer.splice(modulus.size));

  for (int i = _cmp.topOne(exponent); i >= 0; --i) {
    bool one = (exponent.data[i/BufferView::WordSize] >> (i % BufferView::WordSize));
    if(one){
      _mul.kar(value,base,value);
    }
    _mul.kar(value,value,value);
  }
  result.copy(value);
}
