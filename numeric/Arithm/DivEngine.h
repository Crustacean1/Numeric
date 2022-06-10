#ifndef DIV_ENGINE
#define DIV_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt {
class MulEngine;
class AddEngine;
class CompEngine;
class ArithmFacade;

class DivEngine {
  MulEngine &_mul;
  AddEngine &_add;
  CompEngine &_cmp;

  Buffer &_aDivBuffer;
  Buffer &_bDivBuffer;

  DoubleBuffer _buffer;

  void newtonIteration(const BufferView &a, const BufferView &x, size_t prec);
  size_t divApprox(const BufferView &a, const BufferView &x);

public:
  DivEngine(ArithmFacade &arithm);

  void div(const BufferView &dividend, const BufferView &divisor,
           const BufferView &output);

  void fastModulo(const BufferView &arg, const BufferView &modulus,
                  const BufferView &modInverse, const BufferView &result,
                  size_t precision);

  BufferView::BaseInt modulo(const BufferView &arg,
                             BufferView::BaseInt modulus);

  void newtonDiv(const BufferView &dividend, const BufferView &inverse,
                 const BufferView &target, size_t precision);

  size_t newtonInverse(const BufferView &divisor, const BufferView &inverse);
};

} // namespace KCrypt

#endif /*DIV_ENGINE*/
