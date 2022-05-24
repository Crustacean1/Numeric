#ifndef MUL_ENGINE
#define MUL_ENGINE

#include "../Buffer.h"

namespace KCrypt {

class Comparator;
class AddEngine;
class BufferInstance;

class MulEngine {
  DoubleBuffer _buffer;
  void newtonIteration(const IntBuffer &a, const IntBuffer &x, size_t prec);
  size_t divApprox(const IntBuffer &a, const IntBuffer &x);

  Comparator &_comp;
  AddEngine &_adder;

  IntBuffer &karBuffer;
  IntBuffer &divBuffer1;
  IntBuffer &divBuffer2;

  void karIt(const IntBuffer &factorA, const IntBuffer &factorB,
             const IntBuffer &product, const IntBuffer &buffer,
             size_t level = 0);

public:
  MulEngine(Comparator &comp, AddEngine &add, IntBuffer &kBuffer,
            IntBuffer &dBuffer1, IntBuffer &dBuffer2);

  void leftShift(const IntBuffer &source, const IntBuffer &output,
                 size_t offset);
  void rightShift(const IntBuffer &source, const IntBuffer &output,
                  size_t offset);

  void mul(const IntBuffer &factor, const IntBuffer &product);
  void div(const IntBuffer &dividend, const IntBuffer &divisor,
           const IntBuffer &output);

  void mod(const IntBuffer &source, const IntBuffer &modulus,
           const IntBuffer &output);

  void newtonDiv(const IntBuffer &dividend, const IntBuffer &inverse,
                 const IntBuffer &target, size_t precision);

  size_t newtonInverse(const IntBuffer &divisor, const IntBuffer &inverse);

  void kar(const IntBuffer &factorA, const IntBuffer &factorB,
           const IntBuffer &product);

  void modExp(const IntBuffer &base, const IntBuffer &modulus,
              const IntBuffer &exponent, const IntBuffer &result);
};

} // namespace KCrypt
#endif /*MUL_ENGINE*/
