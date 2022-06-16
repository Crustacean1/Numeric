#ifndef ARITHM_FACADE
#define ARITHM_FACADE

#include "ArithmInjector.h"
#include "Buffer/Buffer.h"
#include "Buffer/BufferView.h"
#include <string>

namespace KCrypt {

class ArithmFacade {
  Buffer & _tmpBuffer;

  CompEngine &_cmp;
  AddEngine &_add;
  IoEngine &_io;
  MulEngine &_mul;
  DivEngine &_div;
  GcdEngine &_gcd;
  ExpEngine &_exp;

public:
  ArithmFacade();
  ArithmFacade(const ArithmFacade &) = delete;
  ArithmFacade &operator=(const ArithmFacade &) = delete;

  bool isSigned(const BufferView &view);
  void abs(const BufferView &view);
  void invert(const BufferView &view);

  void extGcd(const BufferView &arg1, const BufferView &arg2,
              const BufferView &output1, const BufferView &ouptut2);

  void modExp(const BufferView &base, const BufferView &exp,
              const BufferView &modulus, const BufferView &output);
  void divide(const BufferView &dividend, const BufferView &divisor,
              const BufferView &output);
  void multiply(const BufferView &factor1, const BufferView &factor2,
                const BufferView &output);
  void leftShift(const BufferView &buffer, size_t shift);
  void rightShift(const BufferView &buffer, size_t shift);

  void subtract(const BufferView &term, const BufferView &outputTerm);
  void subtract(BufferView::BaseInt term, const BufferView &outputTerm);
  void add(const BufferView &term, const BufferView &outputTerm);

  bool equal(const BufferView &a, const BufferView &b);
  bool greater(const BufferView &a, const BufferView &b);
  bool lesser(const BufferView &a, const BufferView &b);
  bool greaterOrEqual(const BufferView &a, const BufferView &b);
  bool lesserOrEqual(const BufferView &a, const BufferView &b);

  void readFromString(const std::string &str, Buffer &buffer);
  std::string writeDecimal(const BufferView &buffer);
  std::string writeBinary(const BufferView &buffer);
};
} // namespace KCrypt

#endif /*ARITHM_FACADE*/
