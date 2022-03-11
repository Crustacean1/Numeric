#ifndef BASICIO
#define BASICIO

#include <random>
#include <cstddef>
#include <string>

#include "buffer.h"

using BaseType = uint32_t;

class BasicIo{
    static std::default_random_engine _engine;
    public:
    static void randomize(Buffer<BaseType> & buffer);
    static std::string parseFromComplement(Buffer<BaseType> & buffer);
    static void parseToComplement(Buffer<BaseType> & buffer, const std::string & str);
};

#endif /*BAISCIO*/