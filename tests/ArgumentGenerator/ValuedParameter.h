#ifndef VALUED_PARAMETER
#define VALUED_PARAMETER

#include "../../tester/argument/ArgumentGenerator.h"
#include <stddef.h>
#include <string>

namespace KCrypt {
class Numeric;
class BasicIo;
class AddEngine;
} // namespace KCrypt

class ValuedParameter : public ArgumentGenerator<KCrypt::Numeric> {
  KCrypt::BasicIo &_io;
  KCrypt::AddEngine &_adder;
  const std::string _value;

public:
  ValuedParameter(KCrypt::BasicIo &io,KCrypt::AddEngine & adder, std::string val);
  KCrypt::Numeric createInstance(std::default_random_engine & e) override;
  ~ValuedParameter();
};

#endif /*VALUED_PARAMETER*/
