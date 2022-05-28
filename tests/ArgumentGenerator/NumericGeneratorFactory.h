#ifndef NUMERIC_GENERATOR_FACTORY
#define NUMERIC_GENERATOR_FACTORY

#include "../../tester/argument/ArgumentGeneratorFactory.h"
#include "../../numeric/Numeric.h"
#include "../../numeric/BasicIo.h"

class NumericGeneratorFactory : public ArgumentGeneratorFactory<KCrypt::Numeric>{
  KCrypt::BasicIo & _io;
  KCrypt::AddEngine & _add;
  public: 
  NumericGeneratorFactory(KCrypt::BasicIo & io, KCrypt::AddEngine & add);
  ArgumentGenerator<KCrypt::Numeric>* create(SyntaxNode & node) override;
};

#endif /*NUMERIC_GENERATOR_FACTORY*/
