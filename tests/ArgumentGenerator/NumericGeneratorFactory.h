#ifndef NUMERIC_GENERATOR_FACTORY
#define NUMERIC_GENERATOR_FACTORY

#include "../../numeric/ArithmFacade.h"
#include "../../tester/argument/ArgumentGeneratorFactory.h"

namespace KCrypt {
class IoEngine;
class AddEngine;
} // namespace KCrypt

class NumericGeneratorFactory
    : public ArgumentGeneratorFactory<KCrypt::Numeric> {
  KCrypt::IoEngine &_io;
  KCrypt::AddEngine &_add;

public:
  NumericGeneratorFactory(KCrypt::ArithmFacade &arithm);
  ArgumentGenerator<KCrypt::Numeric> *create(SyntaxNode &node) override;
};

#endif /*NUMERIC_GENERATOR_FACTORY*/
