#ifndef EXP_ENGINE
#define EXP_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt {

class MulEngine;
class DivEngine;
class AddEngine;
class CompEngine;

class ExpEngine {

  CompEngine &_cmp;
  AddEngine &_add;
  MulEngine &_mul;
  DivEngine &_div;

  Buffer &_buffer3;
  Buffer &_buffer4;

  void reserveModExpBuffers(size_t modulusSize);

public:
  ExpEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul, DivEngine &div,
            Buffer &buffer3, Buffer &buffer4);

  void fastModExp(const BufferView &base, const BufferView &exponent,
                  const BufferView &modulus, const BufferView &modInverse,
                  size_t binPoint, const BufferView &result);
};
} // namespace KCrypt
#endif /*EXP_ENGINE*/
