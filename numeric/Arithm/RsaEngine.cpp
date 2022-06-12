#include <iostream>
#include <random>

#include "../ArithmFacade.h"
#include "ExpEngine.h"
#include "GcdEngine.h"
#include "IoEngine.h"
#include "MulEngine.h"
#include "PrimalityEngine.h"
#include "RsaEngine.h"

using namespace KCrypt;

RsaEngine::RsaEngine(ArithmFacade &arithm)
    : _io(arithm.getIo()), _cmp(arithm.getCmp()), _add(arithm.getAdd()),
      _mul(arithm.getMul()), _div(arithm.getDiv()), _exp(arithm.getExp()),
      _gcd(arithm.getGcd()), _pri(arithm.getPri()), _witness(_buffers[0]),
      _keyExp(_buffers[1]), _keyMod(_buffers[2]), _keyModInv(_buffers[3]),
      _certainityFactor(1),
      _engine(4315), _primes{2,  3,  5,  7,  11, 13, 17, 19, 23, 29, 31, 37,
                             41, 43, 47, 53, 59, 61, 71, 73, 79, 83, 89, 97} {}

bool RsaEngine::isPrime(const BufferView &prime) {
  for (const auto smallPrime : _primes) {
    if (_div.modulo(prime, smallPrime) == 0) {
      return false;
    }
  }

  _pri.setSuspect(prime);

  for (size_t i = 0; i < prime.size * _certainityFactor; ++i) {
    _io.randomize(_witness, _engine, IoEngine::Sign::Unsigned);
    bool testResult = _pri.test(_witness);
    if (!testResult) {
      return false;
    }
  }
  return true;
}

void RsaEngine::generatePrime(const BufferView &prime) {
  prime.clear();
  while (_cmp.leftOffset(prime) > 8) {
    _io.randomize(prime, _engine, IoEngine::Sign::Unsigned);
  }
  prime.data[0] |= BufferView::BaseInt(1);
  //std::cout << "Starting search from: " << _io.toDecimal(prime) << std::endl;
  for (int i = 0; !isPrime(prime); ++i) {
    _add.add(prime, 2);
  }
  //std::cout << "Found: " << _io.toDecimal(prime) << std::endl;
}

// Refactor to accept 2 primes instead for better structuring of parallelism
void RsaEngine::generateKey(size_t keyLength, Buffer &exp1, Buffer &exp2,
                            Buffer &modulus) {
  exp1.reserve(keyLength);
  exp2.reserve(keyLength);
  modulus.reserve(keyLength);

  resizePrimeBuffers(keyLength);

  Buffer prime1(keyLength / 2);
  Buffer prime2(keyLength / 2);

  Buffer invariantExponent(keyLength);

  generatePrime(prime1);
  generatePrime(prime2);

  computeKeyModulus(prime1, prime2, modulus);

  //std::cout << "Modulus computed" << std::endl;
  computePublicKeyExp(exp1);

  //std::cout << "exp computed" << std::endl;
  computeExponentInvariant(prime1, prime2, invariantExponent);

  //std::cout << "inv computed" << std::endl;
  computePrivateKeyExp(exp1, invariantExponent, exp2);
  //std::cout << "exp2 computed" << std::endl;
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

  if(_cmp.isSigned(key)){
    _add.addToRight(invExp,key);
    _add.subFromRight(pubExp,discard);
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
