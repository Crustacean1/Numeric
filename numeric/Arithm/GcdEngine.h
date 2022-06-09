#ifndef GCD_ENGINE
#define GCD_ENGINE

#include "../Buffer/Buffer.h"

namespace KCrypt {

class AddEngine;
class CompEngine;
class ArithmFacade;

class GcdEngine {
  struct GcdExtension {
    const BufferView &value;
    const BufferView &a;
    const BufferView &b;
  };

  Buffer &_a1;
  Buffer &_a2;

  Buffer &_b1;
  Buffer &_b2;

  Buffer &_corr1;
  Buffer &_corr2;

  BufferView _aCorrection;
  BufferView _bCorrection;

  void reserveBuffers(size_t aSize, size_t bSize);
  void gcdDebug(GcdExtension &ext);

  size_t init(GcdExtension &a, const BufferView &view);
  void initCorrections(GcdExtension &a, GcdExtension &b);

  bool iterate(GcdExtension &a, GcdExtension &b);

  void subtract(GcdExtension &small, GcdExtension &big);

  void makeExtensionsEven(GcdExtension &ext);

  void halve(GcdExtension &ext1);

  void computeResult(GcdExtension &ext, size_t offset1, size_t offset2,
                     const BufferView &a, const BufferView &b);
  void reduceOneExtension(GcdExtension &ext, const BufferView &extToHalve,
                          int offset);

  bool isEven(const BufferView &ext);

  CompEngine &_cmp;
  AddEngine &_add;

public:
  GcdEngine(ArithmFacade &arithm);

  void extendedGcd(const BufferView &a, const BufferView &b,
                   const BufferView &aCoeff, const BufferView &bCoeff);
};
} // namespace KCrypt

#endif /*GCD_ENGINE*/
