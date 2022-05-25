#ifndef BASICIO
#define BASICIO

#include <cstddef>
#include <random>
#include <string>

#include "Buffer.h"

class Logger;

namespace KCrypt {

class Comparator;
class AddEngine;

class BasicIo {
  static constexpr size_t bcdBlockSize = 4;
  static constexpr size_t binMax = 16;
  static constexpr size_t decMax = 10;

  size_t toBinSize(size_t size) const;
  size_t toDecSize(size_t size) const;

  void carrySign(unsigned char *input, size_t inputSize) const;

  void normalize(unsigned char *input, size_t unitCount,
                 unsigned char threshold, unsigned char correction) const;

  void shiftLeft(unsigned char *input, size_t inputSize) const;
  void shiftRight(unsigned char *input, size_t inputSize) const;

  char *encodeToAscii(const unsigned char *input, size_t inputSize,
                      bool sign) const;

  unsigned char *decodeFromAscii(const char *input, size_t inputSize) const;

  void buildBinFromSrc(unsigned char *input, size_t inputSize,
                       unsigned char *output) const;
  void buildDecFromSrc(BaseInt *input, size_t inputSize, unsigned char *output,
                       size_t outputSize, bool sign) const;

public:
  BasicIo();
  BasicIo(const BasicIo &);
  BasicIo(BasicIo &&);

  enum class Sign { Random, Signed, Unsigned };

  Buffer<BaseInt> randomize(const Buffer<BaseInt> &num, std::default_random_engine &engine,
                            Sign sign = Sign::Random);

  std::string toDecimal(const Buffer<BaseInt> &buffer,
                        const Comparator &comp) const;
  std::string toBinary(const Buffer<BaseInt> &buffer) const;
  Buffer<BaseInt> toComplement(std::string str, AddEngine &add) const;
};
} // namespace KCrypt

#endif /*BAISCIO*/
