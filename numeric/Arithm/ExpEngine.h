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

  Buffer &_invBuffer;
  Buffer &_expBuffer;
  Buffer &_modBuffer;
  Buffer &_subBuffer;

  void reserveBuffers(size_t modulusSize);

public:
  ExpEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul, DivEngine &div,
            Buffer &buffer1, Buffer &buffer2, Buffer &buffer3, Buffer &buffer4);

  void fastModulo(const BufferView &arg, const BufferView &modulus,
                  const BufferView &modInverse, const BufferView &result,
                  size_t precision);

  void modExp(const BufferView &base, const BufferView &modulus,
              const BufferView &exponent, const BufferView &result);

  void exp(const BufferView &base, const BufferView &exponent,
           const BufferView &result);
};
} // namespace KCrypt
#endif /*EXP_ENGINE*/
