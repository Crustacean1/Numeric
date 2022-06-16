#include <random>

#include "../ArithmInjector.h"
#include "ExpEngine.h"
#include "GcdEngine.h"
#include "IoEngine.h"
#include "MulEngine.h"
#include "PrimalityEngine.h"
#include "RsaEngine.h"

using namespace KCrypt;

RsaEngine::RsaEngine(ArithmInjector &injector)
    : _cmp(injector.getCmp()), _add(injector.getAdd()), _mul(injector.getMul()),
      _div(injector.getDiv()), _exp(injector.getExp()), _gcd(injector.getGcd()),
      _witness(_buffers[0]), _keyExp(_buffers[1]), _keyMod(_buffers[2]),
      _keyModInv(_buffers[3]) {}

// Refactor to accept 2 primes instead for better structuring of parallelism
void RsaEngine::generateKey(const BufferView &prime1, const BufferView &prime2,
                            Buffer &exp1, Buffer &exp2, Buffer &modulus) {

  size_t keyLength = prime1.size + prime2.size;

  exp1.reserve(keyLength);
  exp2.reserve(keyLength);
  modulus.reserve(keyLength);

  resizePrimeBuffers(keyLength);

  Buffer invariantExponent(keyLength);

  computeKeyModulus(prime1, prime2, modulus);

  computePublicKeyExp(exp1);

  computeExponentInvariant(prime1, prime2, invariantExponent);

  computePrivateKeyExp(exp1, invariantExponent, exp2);
}

void RsaEngine::setKey(const BufferView &exp, const BufferView &modulus) {
  resizeKeyBuffers(modulus.size);

  _keyExp.copy(exp);
  _keyMod.copy(modulus);
  _keyModPrec = _div.newtonInverse(_keyMod, _keyModInv);
}

void RsaEngine::apply(const BufferView &input, const BufferView &output) {
  _exp.fastModExp(input, _keyExp, _keyMod, _keyModInv, _keyModPrec, output);
}

void RsaEngine::computePublicKeyExp(const BufferView &buffer) {
  buffer.clear();
  buffer.data[0] = 1;
  buffer.data[0] <<= 16;
  buffer.data[0] += 1;
}

void RsaEngine::computeKeyModulus(const BufferView &prime1,
                                  const BufferView &prime2,
                                  const BufferView &modulus) {
  _mul.kar(prime1, prime2, modulus);
}

void RsaEngine::computeExponentInvariant(const BufferView &prime1,
                                         const BufferView &prime2,
                                         const BufferView &inv) {
  _add.sub(prime1, 1);
  _add.sub(prime2, 1);
  _mul.kar(prime1, prime2, inv);
}

void RsaEngine::computePrivateKeyExp(const BufferView &pubExp,
                                     const BufferView &invExp,
                                     const BufferView &key) {
  key.clear();
  Buffer discard(invExp.size);
  _gcd.extendedGcd(invExp, pubExp, discard, key);

  if (_cmp.isSigned(key)) {
    _add.addToRight(invExp, key);
    _add.subFromRight(pubExp, discard);
  }
}

void RsaEngine::resizeKeyBuffers(size_t keyLength) {
  _buffers[1].resize(keyLength);
  _buffers[2].resize(keyLength);
  _buffers[3].resize(keyLength * 2); // May be too strict...
  _keyExp = _buffers[1].splice(keyLength);
  _keyMod = _buffers[2].splice(keyLength);
  _keyModInv = _buffers[3].splice(keyLength * 2);
}

void RsaEngine::resizePrimeBuffers(size_t keyLength) {
  _buffers[0].resize(keyLength);
  _witness = _buffers[0].splice(keyLength);
}

RsaEngine::~RsaEngine() {}
