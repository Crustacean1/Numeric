#ifndef BASICIO
#define BASICIO

#include <cstddef>
#include <random>
#include <string>

#include "Buffer/Buffer.h"
#include "Buffer/BufferView.h"

class Logger;

namespace KCrypt {

class CompEngine;
class AddEngine;

class IoEngine {
  static constexpr size_t bcdBlockSize = 4;
  static constexpr size_t binMax = 16;
  static constexpr size_t decMax = 10;

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
  void buildDecFromSrc(BufferView::BaseInt *input, size_t inputSize,
                       unsigned char *output, size_t outputSize,
                       bool sign) const;

  CompEngine &_cmp;
  AddEngine &_add;

public:
  IoEngine(CompEngine &cmp, AddEngine &add);
  IoEngine(const IoEngine &);
  IoEngine(IoEngine &&);

  enum class Sign { Random, Signed, Unsigned };

  void randomize(const BufferView &num,
                       std::default_random_engine &engine,
                       Sign sign = Sign::Random);

  size_t decSizeInBinary(size_t size) const;
  size_t binSizeInDecimal(size_t size) const;

  std::string toDecimal(const BufferView &buffer) const;

  std::string toBinary(const BufferView &buffer) const;

  void toComplement(std::string str, const BufferView &view) const;
};
} // namespace KCrypt

#endif /*BAISCIO*/
