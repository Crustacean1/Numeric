#include "RangedParameter.h"
#include "../../numeric/Numeric.h"

RangedParameter::RangedParameter(KCrypt::BasicIo &io, int min, int max)
    : _io(io), _min(min), _max(max) {}

KCrypt::Numeric RangedParameter::createInstance(std::default_random_engine &e) {
  IntBuffer a = IntBuffer::createBuffer(1);
  a.clear();
  std::uniform_int_distribution<BaseInt> dist(_min, _max);
  a.data[0] = dist(e);
  return KCrypt::Numeric(std::move(a));
}
RangedParameter::~RangedParameter(){}
