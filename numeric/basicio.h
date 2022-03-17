#ifndef BASICIO
#define BASICIO

#include <cstddef>
#include <random>
#include <string>

#include "buffer.h"

class Logger;

// TODO: Look into use of intriniscs in parallell correction of bcd
class BasicIo {
  static constexpr size_t bcdBlockSize = 4;
  static constexpr size_t binMax = 16;
  static constexpr size_t decMax = 10;

  std::default_random_engine _engine;

  size_t toBinSize(size_t size);
  size_t toDecSize(size_t size);

  void normalize(unsigned char *input, size_t unitCount, BaseType correction);
  void shiftLeft(unsigned char *input, size_t inputSize);
  void shiftRight(unsigned char *input, size_t inputSize);
  char *decodeToAscii(unsigned char *input, size_t inputSize);

  void toBin(const char *input, size_t inputSize, BaseType *output,
             size_t outputSize);
  void toDec(BaseType *input, size_t inputSize, unsigned char *output,
             size_t outputSize);

  static BasicIo _instance;

public:
  void randomize(Buffer<BaseType> &buffer);
  std::string parseFromComplement(const Buffer<BaseType> &buffer);
  Buffer<BaseType> parseToComplement(const std::string &str);
  BasicIo();

  static BasicIo &getInstance();
};

#endif /*BAISCIO*/
