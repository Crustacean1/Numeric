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
public:
  ExpEngine(CompEngine & cmp, AddEngine & add, MulEngine & mul, DivEngine & div);

  void modExp(const IntBufferView &base, const IntBufferView &modulus,
              const IntBufferView &exponent, const IntBufferView &result);

  void exp(const IntBufferView &base, const IntBufferView &exponent,
           const IntBufferView &result);
};
}
#endif /*EXP_ENGINE*/
