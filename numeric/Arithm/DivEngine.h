#ifndef DIV_ENGINE
#define DIV_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt {
class MulEngine;
class AddEngine;
class CompEngine;

class DivEngine {
  MulEngine &_mul;
  AddEngine &_add;
  CompEngine &_cmp;

  IntBuffer &_aDivBuffer;
  IntBuffer &_bDivBuffer;

  DoubleBuffer _buffer;

  void newtonIteration(const IntBufferView &a, const IntBufferView &x,
                       size_t prec);
  size_t divApprox(const IntBufferView &a, const IntBufferView &x);

public:
  DivEngine(CompEngine &cmp, AddEngine &add, MulEngine &mul,
            IntBuffer &aDivBuffer, IntBuffer &bDivBuff);

  void div(const IntBufferView &dividend, const IntBufferView &divisor,
           const IntBufferView &output);

  void mod(const IntBufferView &source, const IntBufferView &modulus,
           const IntBufferView &output);

  void newtonDiv(const IntBufferView &dividend, const IntBufferView &inverse,
                 const IntBufferView &target, size_t precision);

  size_t newtonInverse(const IntBufferView &divisor,
                       const IntBufferView &inverse);
};

} // namespace KCrypt

#endif /*DIV_ENGINE*/
