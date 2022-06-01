#include "RandomParameter.h"
#include "../../numeric/BasicIo.h"
#include "../../numeric/Numeric.h"

#include <iostream>

using namespace KCrypt;

RandomParameter::RandomParameter(IoEngine &io, size_t size,
                                 const std::string &type)
    : _io(io), _size(size),
      _sign(type == "random"
                ? IoEngine::Sign::Random
                : (type == "signed" ? IoEngine::Sign::Signed
                                    : IoEngine::Sign::Unsigned)) {}

Numeric RandomParameter::createInstance(std::default_random_engine &e) {
  Buffer buffer(_size);
  buffer.clear();
  BufferView view(buffer.splice(_size));

  _io.randomize(view, e, _sign);

  Numeric result(std::move(buffer));
  return result;
}

RandomParameter::~RandomParameter() {}
