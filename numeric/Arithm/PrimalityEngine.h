#ifndef PRIMALITY_ENGINE
#define PRIMALITY_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"
#include <vector>

namespace KCrypt {

class Buffer;
class BufferView;
class Numeric;

class ExpEngine;
class CompEngine;
class AddEngine;
class DivEngine;
class MulEngine;
class ArithmInjector;
class IoEngine;

class PrimalityEngine {
  std::vector<BufferView::BaseInt> _primes;
  float _certainityFactor;

  CompEngine &_cmp;
  AddEngine &_add;
  MulEngine &_mul;
  DivEngine &_div;
  ExpEngine &_exp;

  size_t _binPoint;
  size_t _powerOf2;

  Buffer _modulusBuffer;
  Buffer _modulusInverseBuffer;
  Buffer _mulResultBuffer;
  Buffer _resultBuffer;

  BufferView _modulus;
  BufferView _modulusInverse;
  BufferView _mulResult;
  BufferView _result;

public:
  PrimalityEngine(ArithmInjector & injector);

  bool millerRabinTest(const BufferView &witness);

  bool fastModuloTest(const BufferView &candidate);

  void setSuspect(const BufferView &candidate);
};
} // namespace KCrypt

#endif /*PRIMALITY_ENGINE*/
