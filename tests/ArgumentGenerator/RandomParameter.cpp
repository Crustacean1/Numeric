#include "RandomParameter.h"
#include "../../numeric/BasicIo.h"
#include "../../numeric/Numeric.h"

#include <iostream>

RandomParameter::RandomParameter(KCrypt::BasicIo &io, size_t size,
                                 const std::string &type)
    : _io(io), _size(size),
      _sign(type == "random"
                ? KCrypt::BasicIo::Sign::Random
                : (type == "signed" ? KCrypt::BasicIo::Sign::Signed
                                    : KCrypt::BasicIo::Sign::Unsigned)) {}

KCrypt::Numeric RandomParameter::createInstance(std::default_random_engine &e) {
  Buffer<BaseInt> buffer = Buffer<BaseInt>::createBuffer(_size);
  _io.randomize(buffer,e,_sign);
  KCrypt::Numeric a(buffer);
  return a;
}

RandomParameter::~RandomParameter(){}
