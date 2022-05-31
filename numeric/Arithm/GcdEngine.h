#ifndef GCD_ENGINE
#define GCD_ENGINE

#include "../Buffer/Buffer.h"

namespace KCrypt {

class AddEngine;
class CompEngine;

class GcdEngine {
  struct GcdExtension {
    const BufferView &value;
    const BufferView &a;
    const BufferView &b;
  };

  void reserveBuffers(size_t aSize, size_t bSize);
  void gcdDebug(GcdExtension &ext);

  void init(GcdExtension &a, const BufferView &view);

  bool iterate(GcdExtension &a, GcdExtension &b, BufferView &src1,
               BufferView &src2);

  void subtract(GcdExtension &small, GcdExtension &big);

  void halve(GcdExtension &ext1, const BufferView &src1,
             const BufferView &src2);

  bool isEven(const BufferView &ext);

  CompEngine &_cmp;
  AddEngine &_add;

  Buffer &_a1;
  Buffer &_a2;

  Buffer &_b1;
  Buffer &_b2;

  Buffer &_c;
  Buffer &_d;

public:
  GcdEngine(CompEngine &cmp, AddEngine &add, Buffer &a1, Buffer &a2, Buffer &b1,
            Buffer &b2, Buffer &c, Buffer &d);

  void extendedGcd(const BufferView &a, const BufferView &b,
                   const BufferView &aCoeff, const BufferView &bCoeff);
};
} // namespace KCrypt

#endif /*GCD_ENGINE*/
