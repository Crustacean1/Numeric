#include "RandomParameter.h"
#include "../../numeric/BasicIo.h"
#include "../../numeric/Numeric.h"

#include <iostream>

using namespace KCrypt;

RandomParameter::RandomParameter(BasicIo &io, size_t size,
                                 const std::string &type)
    : _io(io), _size(size),
      _sign(type == "random"
                ? BasicIo::Sign::Random
                : (type == "signed" ? BasicIo::Sign::Signed
                                    : BasicIo::Sign::Unsigned)) {}

Numeric RandomParameter::createInstance(std::default_random_engine &e) {
  Buffer buffer(_size);
  BufferView view(buffer.splice(_size));

  view.clear();
  _io.randomize(view, e, _sign);

  return Numeric(std::move(buffer));
}

RandomParameter::~RandomParameter() {}
