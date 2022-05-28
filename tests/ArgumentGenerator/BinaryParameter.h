#ifndef BINARY_PARAMETER
#define BINARY_PARAMETER

#include "../../numeric/Arithm/AddEngine.h"
#include "../../numeric/Arithm/MulEngine.h"
#include "../../tester/argument/ArgumentGenerator.h"

namespace KCrypt {
class Numeric;
}

class BinaryParameter : public ArgumentGenerator<KCrypt::Numeric> {
  KCrypt::AddEngine &_add;
  std::string _source;

public:
  BinaryParameter(std::string source, KCrypt::AddEngine &add);
  KCrypt::Numeric createInstance(std::default_random_engine &e) override;
  ~BinaryParameter();
};

#endif /*BINARY_PARAMETER*/
