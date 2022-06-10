#ifndef RSA_ENGINE
#define RSA_ENGINE

#include "../Buffer/Buffer.h"
#include <random>
#include <stddef.h>
#include <tuple>

namespace KCrypt {

class Numeric;
class ArithmFacade;

class ExpEngine;
class AddEngine;
class MulEngine;
class DivEngine;
class PrimalityEngine;
class GcdEngine;
class IoEngine;
class CompEngine;

class RsaEngine {
  std::default_random_engine _engine;
  std::vector<BufferView::BaseInt> _primes;
  float _certainityFactor;

  CompEngine &_cmp;
  AddEngine & _add;
  MulEngine & _mul;
  DivEngine & _div;
  ExpEngine &_exp;
  GcdEngine &_gcd;
  IoEngine &_io;
  PrimalityEngine &_pri;

  Buffer & _witnessBuffer;
  BufferView _witness;

  bool isPrime(const BufferView & prime);
  void generatePrime(const BufferView &prime);

public:
  RsaEngine(ArithmFacade &arithm);

  void generateKey(size_t keyLength, Buffer &exp1, Buffer &exp2, Buffer &mod);

  // It must be ensured that input is less then mod, see: block padding
  void apply(const BufferView &input, const BufferView &exp,
             const BufferView &mod, const BufferView &output);
  ~RsaEngine();
};

} // namespace KCrypt
#endif /*RSA_ENGINE*/
