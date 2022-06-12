#ifndef ARGUMENT_GENERATOR
#define ARGUMENT_GENERATOR

#include <random>

template <typename Type> class ArgumentGenerator {
public:
  virtual Type createInstance(std::default_random_engine & engine) = 0;
  virtual ~ArgumentGenerator(){};
};

#endif /*ARGUMENT_GENERATOR*/
