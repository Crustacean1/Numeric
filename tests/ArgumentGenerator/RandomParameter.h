#ifndef RANDOM_PARAMETER
#define RANDOM_PARAMETER

#include "../../tester/argument/ArgumentGenerator.h"
#include "../../numeric/BasicIo.h"
#include <stddef.h>

namespace KCrypt {
class Numeric;
} // namespace KCrypt

class RandomParameter : public ArgumentGenerator<KCrypt::Numeric> {
  KCrypt::BasicIo &_io;
  size_t _size;
  KCrypt::BasicIo::Sign _sign;

public:
  RandomParameter(KCrypt::BasicIo &io, size_t size, const std::string &type);
  KCrypt::Numeric createInstance(std::default_random_engine &engine) override;
  ~RandomParameter();
};

#endif /*RANDOM_PARAMETER*/