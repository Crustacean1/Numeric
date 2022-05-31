#ifndef VALUED_PARAMETER
#define VALUED_PARAMETER

#include "../../tester/argument/ArgumentGenerator.h"
#include <stddef.h>
#include <string>

namespace KCrypt {
class Numeric;
class IoEngine;
class AddEngine;
} // namespace KCrypt

class ValuedParameter : public ArgumentGenerator<KCrypt::Numeric> {
  KCrypt::IoEngine &_io;
  KCrypt::AddEngine &_adder;
  const std::string _value;

public:
  ValuedParameter(KCrypt::IoEngine &io,KCrypt::AddEngine & adder, std::string val);
  KCrypt::Numeric createInstance(std::default_random_engine & e) override;
  ~ValuedParameter();
};

#endif /*VALUED_PARAMETER*/
