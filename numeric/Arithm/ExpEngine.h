#ifndef EXP_ENGINE
#define EXP_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt{

class MulEngine;
class DivEngine;
class AddEngine;
class CompEngine;

class ExpEngine {
  CompEngine & _cmp;
  AddEngine & _add;
  MulEngine & _mul;
  DivEngine & _div;

  Buffer &_invBuffer;
  Buffer &_modBuffer;
public:
  ExpEngine(CompEngine & cmp, AddEngine & add, MulEngine & mul, DivEngine & div, Buffer & buffer1, Buffer & buffer2);

  void modExp(const BufferView &base, const BufferView &modulus,
              const BufferView &exponent, const BufferView &result);

  void exp(const BufferView &base, const BufferView &exponent,
           const BufferView &result);
};
}
#endif /*EXP_ENGINE*/
