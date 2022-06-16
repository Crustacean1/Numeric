#ifndef MOCK_PRIME_GENERATOR
#define MOCK_PRIME_GENERATOR

#include "../numeric/Numeric.h"
#include <random>

class MockPrimeGenerator{
  std::default_random_engine _engine;
  public:
  MockPrimeGenerator();
  KCrypt::Numeric createPrime(size_t primeLength);
};



#endif /*MOCK_PRIME_GENERATOR*/


