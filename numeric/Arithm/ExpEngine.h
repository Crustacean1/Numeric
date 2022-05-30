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
  struct GcdExtension {
    const BufferView &value;
    const BufferView &coefficientA;
    const BufferView &coefficientB;
  };

  CompEngine &_cmp;
  AddEngine &_add;
  MulEngine &_mul;
  DivEngine &_div;

  Buffer &_buffer1;
  Buffer &_buffer2;
  Buffer &_buffer3;
  Buffer &_buffer4;

  void gcdDebug(GcdExtension &ext);

  void init(GcdExtension &a, const BufferView &view, bool position);

  void halve(GcdExtension &small, GcdExtension &big);
  void subtract(GcdExtension &ext1, GcdExtension &ext2, const BufferView &src1,
                const BufferView &src2);
  void reduce(GcdExtension &a);

  bool isReducible(GcdExtension &ext);

  void reserveModExpBuffers(size_t modulusSize);
  void reserveExtGcdBuffers(size_t aSize, size_t bSize);

public:
  ExpEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul, DivEngine &div,
            Buffer &buffer1, Buffer &buffer2, Buffer &buffer3, Buffer &buffer4);

  void fastModulo(const BufferView &arg, const BufferView &modulus,
                  const BufferView &modInverse, const BufferView &result,
                  size_t precision);

  void modExp(const BufferView &base, const BufferView &modulus,
              const BufferView &exponent, const BufferView &result);

  void extendedGcd(const BufferView &a, const BufferView &b,
                   const BufferView &aCoeff, const BufferView &bCoeff);
};
} // namespace KCrypt
#endif /*EXP_ENGINE*/
