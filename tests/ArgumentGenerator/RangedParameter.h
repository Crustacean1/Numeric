#ifndef RANGED_PARAMETER
#define RANGED_PARAMETER

#include "../../tester/argument/ArgumentGenerator.h"
#include <stddef.h>

namespace KCrypt {
class BasicIo;
class Numeric;
} // namespace KCrypt

class RangedParameter : public ArgumentGenerator<KCrypt::Numeric> {
  KCrypt::BasicIo &_io;
  int _min;
  int _max;

public:
  RangedParameter(KCrypt::BasicIo &io, int min, int max);
  KCrypt::Numeric createInstance(std::default_random_engine &e) override;
  ~RangedParameter();
};

#endif /*RANGED_PARAMETER*/
