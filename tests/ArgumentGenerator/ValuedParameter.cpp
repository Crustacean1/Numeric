#include "ValuedParameter.h"
#include "../../numeric/Numeric.h"
//#include "../../numeric/arithm/AddEngine.h"

ValuedParameter::ValuedParameter(KCrypt::BasicIo &io, KCrypt::AddEngine &adder,
                                 std::string val)
    : _io(io), _adder(adder), _value(val) {}

KCrypt::Numeric ValuedParameter::createInstance(std::default_random_engine &e) {
  IntBuffer buffer = _io.toComplement(_value, _adder);
  return KCrypt::Numeric(std::move(buffer));
}
ValuedParameter::~ValuedParameter(){}
