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

  void modExp(const Buffer &base, const Buffer &modulus,
              const Buffer &exponent, const Buffer &result);

  void exp(const Buffer &base, const Buffer &exponent,
           const Buffer &result);
};
}
#endif /*EXP_ENGINE*/
