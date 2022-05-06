#include "basicio.h"
#include "arithm.h"
#include <iostream>
#include <ostream>
#include <string>

BasicIo BasicIo::_instance;

BasicIo::BasicIo() : _engine(2137) {}

BasicIo &BasicIo::getInstance() { return _instance; }

void displayString(unsigned char *input, size_t inputSize) {
  for (size_t i = 0; i < inputSize; ++i) {
    std::cout << (int)input[i] << " ";
  }
  std::cout << std::endl;
}

Buffer<BaseType> BasicIo::createRandom(size_t size, bool isSigned) {
  auto buffer = Buffer<BaseType>::createBuffer(size);
  buffer.clear();
  std::uniform_int_distribution<BaseType> dist(0, -1);
  for (size_t i = 0; i < size; ++i) {
    buffer.data[i] = dist(_engine);
  }
  if (isSigned) {
    buffer.data[size-1] |= (BaseType(1) << (sizeof(BaseType) * 8 - 1));
  } else {
    buffer.data[size-1] &=
        (~(BaseType(1) << (sizeof(BaseType) * 8 - 1)));
  }
  return buffer;
}

Buffer<BaseType> BasicIo::createRandom(size_t size) {
  auto buffer = Buffer<BaseType>::createBuffer(size);
  buffer.clear();
  std::uniform_int_distribution<BaseType> dist(0, -1);
  for (size_t i = 0; i < buffer.size; ++i) {
    buffer.data[i] = dist(_engine);
  }
  return buffer;
}

Buffer<BaseType> BasicIo::createRandom(size_t min, size_t max) {
  auto buffer = Buffer<BaseType>::createBuffer(1);
  buffer.clear();
  buffer.data[0] = std::uniform_int_distribution<BaseType>(min, max)(_engine);
  return buffer;
}

Buffer<BaseType> BasicIo::toBuffer(const std::string &str, Arithm &arth) {
  bool sign = str[0] == '-';

  auto sourceSize = (str.size() - 2 * sign + 1) / 2;
  auto bcdSource = decodeFromAscii(str.c_str() + sign, str.size() - sign);

  auto buffer = Buffer<BaseType>::createBuffer(toBinSize(str.size()));
  buffer.clear();

  toBin(bcdSource, sourceSize, (unsigned char *)buffer.data);

  if (sign) {
    arth.invert(buffer);
  }

  delete[] bcdSource;
  return buffer;
}

std::string BasicIo::getDec(const Buffer<BaseType> &buffer, Arithm &arth) {
  bool sign = arth.isSigned(buffer);
  sign = false;

  size_t outputSize = toDecSize(buffer.size);
  unsigned char *output = new unsigned char[outputSize + 1];
  memset(output, 0, outputSize);

  toDec(buffer.data, buffer.size, output, outputSize, sign);
  auto result = encodeToAscii(output, outputSize, sign);

  delete[] output;
  return std::string(result);
}

std::string BasicIo::getBin(const Buffer<BaseType> &buffer, Arithm &arth) {
  constexpr size_t wordSize = sizeof(BaseType) * 8;
  std::string result;
  for (size_t i = buffer.size - 1; i >= 0; --i) {
    for (size_t j = 0; j < wordSize; ++j) {
      result.insert(result.begin(), (char)(((buffer.data[i] >> j) & 1) + '0'));
    }
  }
  return result;
}

size_t BasicIo::toBinSize(size_t size) {
  return (size * 3 + 3) / (4 * sizeof(BaseType));
}

size_t BasicIo::toDecSize(size_t size) {
  return (size * sizeof(BaseType) * 4 + 2) / 3;
}

inline void BasicIo::shiftRight(unsigned char *input, size_t inputSize) {
  constexpr size_t wordSize = sizeof(char) * 8;

  BufferType buffer = input[0];
  BufferType accumulator;

  for (size_t i = 0; i < inputSize - 1; ++i) {
    accumulator = input[i + 1];
    accumulator <<= wordSize;
    buffer += accumulator;
    input[i] = ((buffer >> 1) & 255);
    buffer >>= wordSize;
  }
  input[inputSize - 1] >>= 1;
}

inline void BasicIo::shiftLeft(unsigned char *input, size_t inputSize) {
  BufferType buffer = input[inputSize - 1];
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
                               unsigned char correction) {
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
                             bool sign) {
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

unsigned char *BasicIo::decodeFromAscii(const char *input, size_t inputSize) {
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

void BasicIo::toDec(BaseType *input, size_t inputSize, unsigned char *output,
                    size_t outputSize, bool sign) {
  size_t wordBitSize = sizeof(BaseType) * 8;
  size_t correction = (binMax - decMax) / 2;

  BaseType buffer = 0;

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

void BasicIo::carrySign(unsigned char *input, size_t inputSize) {
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

void BasicIo::toBin(unsigned char *input, size_t inputSize,
                    unsigned char *output) {
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
