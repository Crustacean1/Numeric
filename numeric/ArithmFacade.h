#ifndef ARITHM_FACADE
#define ARITHM_FACADE

#include "Arithm/AddEngine.h"
#include "Arithm/CompEngine.h"
#include "Arithm/DivEngine.h"
#include "Arithm/ExpEngine.h"
#include "Arithm/GcdEngine.h"
#include "Arithm/MulEngine.h"
#include "BasicIo.h"

#include "Buffer/BufferInstance.h"

namespace KCrypt {

class ArithmFacade {
  ArithmFacade();

  BufferInstance _buffInst;

  IoEngine _io;
  CompEngine _cmp;
  AddEngine _add;
  MulEngine _mul;
  DivEngine _div;
  ExpEngine _exp;
  GcdEngine _gcd;

public:
  static ArithmFacade &getInstance(size_t threadId);

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
  void leftShift(const BufferView &buffer, size_t shift,
                 const BufferView &output);
  void rightShift(const BufferView &buffer, size_t shift,
                  const BufferView &output);

  void subtract(const BufferView &term, const BufferView &outputTerm);
  void add(const BufferView &term, const BufferView &outputTerm);

  bool equal(const BufferView &a, const BufferView &b);
  bool greater(const BufferView &a, const BufferView &b);
  bool lesser(const BufferView &a, const BufferView &b);
  bool greaterOrEqual(const BufferView &a, const BufferView &b);
  bool lesserOrEqual(const BufferView &a, const BufferView &b);

  void readFromString(const std::string &str, Buffer &buffer);
  std::string writeToString(const BufferView &buffer);

  bool millerRabinTest(const BufferView &suspect, const BufferView &witness);
};
} // namespace KCrypt

#endif /*ARITHM_FACADE*/
