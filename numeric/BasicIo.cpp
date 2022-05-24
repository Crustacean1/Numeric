#include "BasicIo.h"
#include "arithm/AddEngine.h"
#include "arithm/Comparator.h"
#include <iostream>
#include <ostream>
#include <string>

namespace KCrypt {

BasicIo::BasicIo() {}

void displayString(unsigned char *input, size_t inputSize) {
  for (size_t i = 0; i < inputSize; ++i) {
    std::cout << (int)input[i] << " ";
  }
  std::cout << std::endl;
}

Buffer<BaseInt> BasicIo::randomize(Buffer<BaseInt> &buffer,
                                   std::default_random_engine &engine,
                                   Sign sign) {
  std::uniform_int_distribution<BaseInt> dist(0, 2);
  for (size_t i = 0; i < buffer.size; ++i) {
    for (int j = 0; j < sizeof(BaseInt) * 8; ++j) {
      buffer.data[i] += dist(engine);
      buffer.data[i] <<= 1;
    }
  }
  BaseInt signCorrection = 1;
  signCorrection <<= highWordPos;
  if (sign == Sign::Signed) {
    buffer.data[buffer.size - 1] |= signCorrection;
  }
  if (sign == Sign::Unsigned) {
    buffer.data[buffer.size - 1] &= (~signCorrection);
  }

  return buffer;
}

Buffer<BaseInt> BasicIo::toComplement(std::string str,
                                      AddEngine &adder) const {
  bool sign = str[0] == '-';

  auto sourceSize = (str.size() - 2 * sign + 1) / 2;
  auto bcdSource = decodeFromAscii(str.c_str() + sign, str.size() - sign);

  auto buffer = Buffer<BaseInt>::createBuffer(toBinSize(str.size()));
  buffer.clear();

  buildBinFromSrc(bcdSource, sourceSize, (unsigned char *)buffer.data);

  if (sign) {
    adder.invert(buffer);
  }

  delete[] bcdSource;
  return buffer;
}

std::string BasicIo::toDecimal(const Buffer<BaseInt> &buffer,
                               const Comparator &comp) const {
  bool sign = comp.isSigned(buffer);

  size_t outputSize = toDecSize(buffer.size);
  unsigned char *output = new unsigned char[outputSize + 1];
  memset(output, 0, outputSize);

  buildDecFromSrc(buffer.data, buffer.size, output, outputSize, sign);
  auto charResult = encodeToAscii(output, outputSize, sign);

  std::string result(charResult);
  delete[] output;
  delete[] charResult;
  return result;
}

std::string BasicIo::toBinary(const Buffer<BaseInt> &buffer) const {
  constexpr size_t wordSize = sizeof(BaseInt) * 8;
  std::string result;
  for (size_t i = buffer.size; i > 0; --i) {
    for (size_t j = wordSize; j > 0; --j) {
      result += (char)(((buffer.data[i - 1] >> (j - 1)) & 1) + '0');
    }
  }
  return result;
}

size_t BasicIo::toBinSize(size_t size) const {
  return (size * 3 + 3) / (4 * sizeof(BaseInt));
}

size_t BasicIo::toDecSize(size_t size) const {
  return (size * sizeof(BaseInt) * 4 + 2) / 3;
}

inline void BasicIo::shiftRight(unsigned char *input, size_t inputSize) const {
  constexpr size_t wordSize = sizeof(char) * 8;

  BufferInt buffer = input[0];
  BufferInt accumulator;

  for (size_t i = 0; i < inputSize - 1; ++i) {
    accumulator = input[i + 1];
    accumulator <<= wordSize;
    buffer += accumulator;
    input[i] = ((buffer >> 1) & 255);
    buffer >>= wordSize;
  }
  input[inputSize - 1] >>= 1;
}

inline void BasicIo::shiftLeft(unsigned char *input, size_t inputSize) const {
  BufferInt buffer = input[inputSize - 1];
  constexpr size_t wordSize = sizeof(char) * 8;
  for (size_t i = inputSize - 1; i > 0; --i) {
    buffer <<= wordSize;
    buffer += input[i - 1];
    input[i] = ((buffer >> (wordSize - 1)) & 255);
  }
  input[0] = (input[0] << 1);
}

inline void BasicIo::normalize(unsigned char *input, size_t inputSize,
                               unsigned char threshold,
                               unsigned char correction) const {
  size_t blocksInWord = sizeof(char) * 2;

  unsigned char accumulator = 0;
  unsigned char buffer = 0;

  for (size_t i = 0; i < inputSize; ++i, accumulator = 0) {
    for (size_t j = blocksInWord; j > 0; --j) {
      buffer = (input[i] >> ((j - 1) * 4)) & (15);
      accumulator <<= 4;
      if (buffer > threshold) {
        accumulator += (buffer + correction) & 15;
      } else {
        accumulator += buffer;
      }
    }
    input[i] = accumulator;
  }
}

char *BasicIo::encodeToAscii(const unsigned char *input, size_t inputSize,
                             bool sign) const {
  for (; inputSize > 1 && input[inputSize - 1] == 0; --inputSize) {
  }

  char *asciiDecoded = new char[inputSize * 2 + 1 + sign];
  asciiDecoded[0] = '-';

  size_t arrayPos;

  for (size_t i = 0; i < inputSize; ++i) {
    arrayPos = ((inputSize - 1 - i) * 2) + sign;
    asciiDecoded[arrayPos] = '0' + ((input[i] >> 4));
    asciiDecoded[arrayPos + 1] = '0' + (input[i] & 15);
  }

  asciiDecoded[inputSize * 2 + sign] = 0;

  return asciiDecoded;
}

unsigned char *BasicIo::decodeFromAscii(const char *input,
                                        size_t inputSize) const {
  size_t bcdSize = (inputSize + 1) / 2;
  unsigned char *packedBcd = new unsigned char[bcdSize];
  memset(packedBcd, 0, bcdSize);

  for (size_t i = 0; i < inputSize; ++i) {
    packedBcd[i / 2] >>= 4;
    packedBcd[i / 2] += (((unsigned char)input[inputSize - 1 - i] - '0') << 4);
  }
  if (inputSize & 1) {
    packedBcd[inputSize / 2] >>= 4;
  }
  return packedBcd;
}

void BasicIo::buildDecFromSrc(BaseInt *input, size_t inputSize,
                              unsigned char *output, size_t outputSize,
                              bool sign) const {
  size_t wordBitSize = sizeof(BaseInt) * 8;
  size_t correction = (binMax - decMax) / 2;

  BaseInt buffer = 0;

  for (size_t i = inputSize; i > 0; --i) {
    if (sign) {
      buffer = ~input[i - 1];
    } else {
      buffer = input[i - 1];
    }
    for (size_t j = wordBitSize; j > 0; --j) {
      normalize(output, outputSize, 4, correction);
      shiftLeft(output, outputSize);
      output[0] += (buffer >> (j - 1)) & 1;
    }
  }
  if (sign) {
    carrySign(output, inputSize);
  }
}

void BasicIo::carrySign(unsigned char *input, size_t inputSize) const {
  unsigned char carry = 1;
  for (size_t i = 0; i < inputSize && carry != 0; ++i) {
    if ((input[i] & 15) < 9) {
      input[i] += 1;
      break;
    }
    input[i] &= 240;
    if ((input[i] >> 4) < 9) {
      input[i] += 16;
      break;
    }
    input[i] &= 15;
  }
}

void BasicIo::buildBinFromSrc(unsigned char *input, size_t inputSize,
                              unsigned char *output) const {
  size_t correction = (binMax - (binMax - decMax) / 2);

  for (size_t i = 0; i < inputSize; ++i) {
    output[i] = 0;
    for (size_t j = 8; j > 0; --j) {
      output[i] >>= 1;
      output[i] += ((input[0] & 1) << 7);
      shiftRight(input, inputSize);
      normalize(input, inputSize, 7, correction);
    }
  }
}
} // namespace KCrypt
