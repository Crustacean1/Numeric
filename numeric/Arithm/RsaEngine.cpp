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
      _gcd(arithm.getGcd()), _pri(arithm.getPri()),
      _witnessBuffer(arithm.getBuffer(9)), _witness(_witnessBuffer),
      _certainityFactor(1),
      _engine(3214), _primes{2,  3,  5,  7,  11, 13, 17, 19, 23, 29, 31, 37,
                             41, 43, 47, 53, 59, 61, 71, 73, 79, 83, 89, 97} {}

bool RsaEngine::isPrime(const BufferView &prime) {
  for (const auto smallPrime : _primes) {
    if (_div.modulo(prime, smallPrime) == 0) {
      return false;
    }
  }

  _pri.setSuspect(prime);

  for (size_t i = 0; i < prime.size * _certainityFactor; ++i) {
    _io.randomize(_witness, _engine, IoEngine::Sign::Random);
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
    _io.randomize(prime, _engine, IoEngine::Sign::Random);
  }
  prime.data[0] |= BufferView::BaseInt(1);
  while (!isPrime(prime)) {
    _add.add(prime, 2);
  }
}

void RsaEngine::generateKey(size_t keyLength, Buffer &exp1, Buffer &exp2,
                            Buffer &modulus) {
  exp1.reserve(keyLength);
  exp2.reserve(keyLength);
  modulus.reserve(keyLength);

  _witnessBuffer.reserve(keyLength / 2);
  _witness = _witnessBuffer.splice(keyLength / 2);

  Buffer prime1(keyLength / 2);
  Buffer prime2(keyLength / 2);
  Buffer invariantExponent(keyLength);

  generatePrime(prime1);
  generatePrime(prime2);

  _mul.kar(prime1, prime2, modulus);

  _add.sub(prime1, 1);
  _add.sub(prime2, 1);
  _mul.kar(prime1, prime2, invariantExponent);

  size_t invariantOffset = _cmp.rightOffset(invariantExponent);
  _add.rightShift(invariantExponent, invariantExponent, invariantOffset);

}

RsaEngine::~RsaEngine() {}
