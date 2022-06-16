#ifndef EXP_ENGINE
#define EXP_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt {

class MulEngine;
class DivEngine;
class AddEngine;
class CompEngine;
class ArithmInjector;
class IoEngine;

class ExpEngine {

  CompEngine &_cmp;
  AddEngine &_add;
  MulEngine &_mul;
  DivEngine &_div;
  IoEngine & _io;

  Buffer _buffer;
  BufferView _value;

  void reserveModExpBuffers(size_t modulusSize);

public:
  ExpEngine(ArithmInjector & injector);

  void fastModExp(const BufferView &base, const BufferView &exponent,
                  const BufferView &modulus, const BufferView &modInverse,
                  size_t binPoint, const BufferView &result);
};
} // namespace KCrypt
#endif /*EXP_ENGINE*/
