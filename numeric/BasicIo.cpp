#include <cstring>
#include <iostream>
#include <ostream>
#include <string>

#include "Arithm/AddEngine.h"
#include "Arithm/CompEngine.h"
#include "ArithmFacade.h"
#include "BasicIo.h"

namespace KCrypt {

IoEngine::IoEngine(ArithmFacade &arithm)
    : _cmp(arithm.getCmp()), _add(arithm.getAdd()) {}

void displayString(unsigned char *input, size_t inputSize) {
  for (size_t i = 0; i < inputSize; ++i) {
    std::cout << (int)input[i] << " ";
  }
  std::cout << std::endl;
}

void IoEngine::randomize(const BufferView &buffer,
                         std::default_random_engine &engine, Sign sign) {
  std::uniform_int_distribution<BufferView::BaseInt> dist(0, 2);
  for (size_t i = 0; i < buffer.size; ++i) {
    for (int j = 0; j < sizeof(BufferView::BaseInt) * 8; ++j) {
      buffer.data[i] += dist(engine);
      buffer.data[i] <<= 1;
    }
  }
  BufferView::BaseInt signCorrection = 1;
  signCorrection <<= BufferView::WordHighBit;
  if (sign == Sign::Signed) {
    buffer.data[buffer.size - 1] |= signCorrection;
  }
  if (sign == Sign::Unsigned) {
    buffer.data[buffer.size - 1] &= (~signCorrection);
  }
}

void IoEngine::toComplement(std::string str, const BufferView &view) const {
  bool sign = str[0] == '-';

  auto compactedDigitCount = (str.size() - 2 * sign + 1) / 2;
  auto compactedDigits = decodeFromAscii(str.c_str() + sign, str.size() - sign);

  view.clear();

  buildBinFromSrc(compactedDigits, compactedDigitCount,
                  (unsigned char *)view.data);

  if (sign) {
    _add.invert(view);
  }

  delete[] compactedDigits;
}

std::string IoEngine::toDecimal(const BufferView &buffer) const {
  bool sign = _cmp.isSigned(buffer);
  // sign = false;

  size_t outputSize = binSizeInDecimal(buffer.size);
  unsigned char *output = new unsigned char[outputSize + 1];
  memset(output, 0, outputSize);

  buildDecFromSrc(buffer.data, buffer.size, output, outputSize, sign);
  auto charResult = encodeToAscii(output, outputSize, sign);

  std::string result(charResult);
  delete[] output;
  delete[] charResult;
  return result;
}

std::string IoEngine::toBinary(const BufferView &buffer) const {
  constexpr size_t wordSize = sizeof(BufferView::BaseInt) * 8;
  std::string result;
  for (size_t i = buffer.size; i > 0; --i) {
    for (size_t j = wordSize; j > 0; --j) {
      result += (char)(((buffer.data[i - 1] >> (j - 1)) & 1) + '0');
    }
  }
  return result;
}

size_t IoEngine::decSizeInBinary(size_t size) const {
  return (size * 3 + 3) / (4 * sizeof(BufferView::BufferView::BaseInt));
}

size_t IoEngine::binSizeInDecimal(size_t size) const {
  return (size * sizeof(BufferView::BufferView::BaseInt) * 4 + 2) / 3;
}

inline void IoEngine::shiftRight(unsigned char *input, size_t inputSize) const {
  constexpr size_t wordSize = sizeof(char) * 8;

  BufferView::BufferInt buffer = input[0];
  BufferView::BufferInt accumulator;

  for (size_t i = 0; i < inputSize - 1; ++i) {
    accumulator = input[i + 1];
    accumulator <<= wordSize;
    buffer += accumulator;
    input[i] = ((buffer >> 1) & 255);
    buffer >>= wordSize;
  }
  input[inputSize - 1] >>= 1;
}

inline void IoEngine::shiftLeft(unsigned char *input, size_t inputSize) const {
  BufferView::BufferInt buffer = input[inputSize - 1];
  constexpr size_t wordSize = sizeof(char) * 8;
  for (size_t i = inputSize - 1; i > 0; --i) {
    buffer <<= wordSize;
    buffer += input[i - 1];
    input[i] = ((buffer >> (wordSize - 1)) & 255);
  }
  input[0] = (input[0] << 1);
}

inline void IoEngine::normalize(unsigned char *input, size_t inputSize,
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

char *IoEngine::encodeToAscii(const unsigned char *input, size_t inputSize,
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

unsigned char *IoEngine::decodeFromAscii(const char *input,
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

void IoEngine::buildDecFromSrc(BufferView::BaseInt *input, size_t inputSize,
                               unsigned char *output, size_t outputSize,
                               bool sign) const {
  size_t wordBitSize = sizeof(BufferView::BaseInt) * 8;
  size_t correction = (binMax - decMax) / 2;

  BufferView::BaseInt buffer = 0;

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

void IoEngine::carrySign(unsigned char *input, size_t inputSize) const {
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

void IoEngine::buildBinFromSrc(unsigned char *input, size_t inputSize,
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
