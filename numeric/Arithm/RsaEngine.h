#ifndef RSA_ENGINE
#define RSA_ENGINE

#include "../Buffer/Buffer.h"
#include <random>
#include <stddef.h>
#include <tuple>

namespace KCrypt {

class Numeric;
class ArithmInjector;

class ExpEngine;
class AddEngine;
class MulEngine;
class DivEngine;
class PrimalityEngine;
class GcdEngine;
class IoEngine;
class CompEngine;

class RsaEngine {
  CompEngine &_cmp;
  AddEngine &_add;
  MulEngine &_mul;
  DivEngine &_div;
  ExpEngine &_exp;
  GcdEngine &_gcd;

  Buffer _buffers[5];

  BufferView _witness;
  BufferView _keyExp;
  BufferView _keyMod;
  BufferView _keyModInv;
  BufferView _inputBuffer;

  size_t _keyModPrec;

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
  RsaEngine(ArithmInjector &injector);

  void generateKey(const BufferView &prime1, const BufferView &prime2,
                   Buffer &exp1, Buffer &exp2, Buffer &mod);

  void setKey(const BufferView &exp, const BufferView &modulus);

  // Input must be padded beforehand: [data][random 8 bits][empty 8 bits]
  void apply(const BufferView &input, const BufferView &output);

  ~RsaEngine();
};

} // namespace KCrypt
#endif /*RSA_ENGINE*/
