#include "mock_prime_generator.h"
#include "../numeric/Arithm/IoEngine.h"
#include "../numeric/Arithm/PrimalityEngine.h"
#include "../numeric/ArithmInjector.h"
#include <iostream>
#include <random>

MockPrimeGenerator::MockPrimeGenerator() : _engine(2137) {}
KCrypt::Numeric MockPrimeGenerator::createPrime(size_t primeLength) {

  auto &io = KCrypt::ArithmInjector::getInstance().getIo();
  auto &add = KCrypt::ArithmInjector::getInstance().getAdd();
  KCrypt::PrimalityEngine prim(KCrypt::ArithmInjector::getInstance());

  KCrypt::Numeric prime(primeLength);
  io.randomize(prime.getBuffer(), _engine, KCrypt::IoEngine::Sign::Unsigned);
  prime.getBuffer().data[0] |= 1;

  std::vector<KCrypt::Buffer> witnesses;
  for (size_t i = 0; i < primeLength; ++i) {
    witnesses.emplace_back(primeLength);
    io.randomize(witnesses.back(), _engine, KCrypt::IoEngine::Sign::Unsigned);
  }

  size_t j = 0;
  for (size_t i;;) {

    if (prim.fastModuloTest(prime.getBuffer())) {

      prim.setSuspect(prime.getBuffer());

      for (i = 0; i < witnesses.size() && prim.millerRabinTest(witnesses[i]);
           ++i) {
      }

      if (i == witnesses.size()) {
        return prime;
      }
    }
    add.add(prime.getBuffer(), 2);
  }
}
