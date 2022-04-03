#ifndef BASICIO
#define BASICIO

#include <cstddef>
#include <random>
#include <string>

#include "buffer.h"

class Logger;
class Arithm;

// TODO: Look into use of intriniscs in parallel correction of bcd

class BasicIo {
  static constexpr size_t bcdBlockSize = 4;
  static constexpr size_t binMax = 16;
  static constexpr size_t decMax = 10;

  std::default_random_engine _engine;

  size_t toBinSize(size_t size);
  size_t toDecSize(size_t size);

  void carrySign(unsigned char *input, size_t inputSize);

  void normalize(unsigned char *input, size_t unitCount,
                 unsigned char threshold, unsigned char correction);

  void shiftLeft(unsigned char *input, size_t inputSize);
  void shiftRight(unsigned char *input, size_t inputSize);

  char *encodeToAscii(const unsigned char *input, size_t inputSize, bool sign);
  unsigned char *decodeFromAscii(const char *input, size_t inputSize);

  void toBin(unsigned char *input, size_t inputSize, unsigned char *output);
  void toDec(BaseType *input, size_t inputSize, unsigned char *output,
             size_t outputSize, bool sign);

  static BasicIo _instance;

  BasicIo();
  BasicIo(const BasicIo &);
  BasicIo(BasicIo &&);

public:
  Buffer<BaseType> createRandom(size_t size);
  Buffer<BaseType> createRandom(size_t size, bool isSigned);
  Buffer<BaseType> createRandom(size_t min, size_t max);

  std::string getDec(const Buffer<BaseType> &buffer, Arithm &arth);
  std::string getBin(const Buffer<BaseType> &buffer, Arithm &arth);
  Buffer<BaseType> toBuffer(const std::string &str, Arithm &arth);

  static BasicIo &getInstance();
};

#endif /*BAISCIO*/
