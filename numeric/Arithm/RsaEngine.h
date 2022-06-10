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
  AddEngine &_add;
  MulEngine &_mul;
  DivEngine &_div;
  ExpEngine &_exp;
  GcdEngine &_gcd;
  IoEngine &_io;
  PrimalityEngine &_pri;

  Buffer _buffers[4];

  BufferView _witness;
  BufferView _keyExp;
  BufferView _keyMod;
  BufferView _keyModInv;

  size_t _keyModPrec;

  bool isPrime(const BufferView &prime);
  void generatePrime(const BufferView &prime);

  void computeKeyModulus(const BufferView &prime1, const BufferView &prime2,
                         const BufferView &modulus);

  void computeExponentInvariant(const BufferView &fct1, const BufferView &fct2,
                                const BufferView &inv);

  void computePrivateKeyExp(const BufferView &privExp, const BufferView &invExp,
                            const BufferView &key);

  void computePublicKeyExp(const BufferView &buffer);

  void resizePrimeBuffers(size_t primeSize);
  void resizeKeyBuffers(size_t keySize);

public:
  RsaEngine(ArithmFacade &arithm);

  void generateKey(size_t keyLength, Buffer &exp1, Buffer &exp2, Buffer &mod);

  void setKey(const BufferView &exp, const BufferView &modulus);

  // Input must be padded beforehand: [data][random 8 bits][empty 8 bits]
  void apply(const BufferView &input, const BufferView &output);

  ~RsaEngine();
};

} // namespace KCrypt
#endif /*RSA_ENGINE*/
