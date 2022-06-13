#ifndef PRIMALITY_ENGINE
#define PRIMALITY_ENGINE

#include <random>

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt {

class Buffer;
class BufferView;
class Numeric;

class ExpEngine;
class CompEngine;
class AddEngine;
class DivEngine;
class MulEngine;
class ArithmFacade;
class IoEngine;

class PrimalityEngine {
  static constexpr size_t _primeCount = 24;
  size_t _primes[_primeCount];

  float _certainityFactor = 1.f;

  CompEngine &_cmp;
  AddEngine &_add;
  MulEngine &_mul;
  DivEngine &_div;
  ExpEngine &_exp;
  IoEngine &_io;

  size_t _binPoint;
  size_t _powerOf2;

  Buffer _modulusBuffer;
  Buffer _modulusInverseBuffer;
  Buffer _mulResultBuffer;
  Buffer _resultBuffer;
  Buffer _witnessBuffer;

  BufferView _modulusInverse;
  BufferView _modulus;
  BufferView _mulResult;
  BufferView _result;
  BufferView _witness;

public:
  PrimalityEngine(ArithmFacade &arithm);

  bool millerRabinTest(const BufferView &witness);
  void setSuspect(const BufferView &buffer);

  bool fastPrimeTest(const BufferView &candidate);
};
} // namespace KCrypt

#endif /*PRIMALITY_ENGINE*/
