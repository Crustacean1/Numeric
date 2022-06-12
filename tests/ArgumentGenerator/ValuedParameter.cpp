#include "ValuedParameter.h"
#include "../../numeric/Numeric.h"

using namespace KCrypt;

ValuedParameter::ValuedParameter(IoEngine &io, AddEngine &adder,
                                 std::string val)
    : _io(io), _adder(adder), _value(val) {}

Numeric ValuedParameter::createInstance(std::default_random_engine &e) {
  Buffer buffer(_io.decSizeInBinary(_value.size()));
  BufferView view(buffer.splice());
  _io.toComplement(_value, view);
  return Numeric(std::move(buffer));
}
ValuedParameter::~ValuedParameter() {}
