#include "RangedParameter.h"
#include "../../numeric/Numeric.h"

using namespace KCrypt;

RangedParameter::RangedParameter(IoEngine &io, int min, int max)
    : _io(io), _min(min), _max(max) {}

Numeric RangedParameter::createInstance(std::default_random_engine &e) {
  Buffer buffer(1);
  BufferView view(buffer.splice());
  view.clear();
  std::uniform_int_distribution<BufferView::BaseInt> dist(_min, _max);
  view.data[0] = dist(e);
  return Numeric(std::move(buffer));
}
RangedParameter::~RangedParameter(){}
