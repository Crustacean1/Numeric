#include "basicio.h"
#include <iostream>
#include <ostream>

BasicIo BasicIo::_instance;

BasicIo::BasicIo() : _engine(2137) {}

BasicIo &BasicIo::getInstance() { return _instance; }

void displayString(unsigned char *input, size_t inputSize) {
  for (size_t i = 0; i < inputSize; ++i) {
    std::cout << (int)input[i] << " ";
  }
  std::cout << std::endl;
}

void BasicIo::randomize(Buffer<BaseType> &buffer) {
  std::uniform_int_distribution<BaseType> dist(0, -1);
  for (size_t i = 0; i < buffer.size; ++i) {
    buffer.data[i] = dist(_engine);
  }
}

Buffer<BaseType> BasicIo::parseToComplement(const std::string &str) {
  auto buffer = Buffer<BaseType>::createBuffer(0);
  buffer.data[0] = 2137; // binary algo, todo
  return buffer;
}

std::string BasicIo::parseFromComplement(const Buffer<BaseType> &buffer) {
  size_t outputSize = toDecSize(buffer.size);
  unsigned char *output = new unsigned char[outputSize + 1];
  memset(output, 0, outputSize);

  toDec(buffer.data, buffer.size, output, outputSize);
  auto result = decodeToAscii(output, outputSize);
  delete[] output;
  return std::string(result);
}

size_t BasicIo::toBinSize(size_t size) { return size * 0.997 + 1; }
size_t BasicIo::toDecSize(size_t size) { return size * 1.004 + 1; }

void BasicIo::shiftRight(unsigned char *input, size_t inputSize) {
  BufferType buffer = input[1];
  const size_t wordSize = sizeof(char) * 8;
  for (size_t i = 0; i < inputSize - 1; ++i) {
    buffer <<= wordSize;
    buffer += input[i];
    input[i] = ((buffer >> 1) & 255);
  }
  input[inputSize] = (input[inputSize] << 1);
}

void BasicIo::shiftLeft(unsigned char *input, size_t inputSize) {
  BufferType buffer = input[inputSize - 1];
  const size_t wordSize = sizeof(char) * 8;
  for (size_t i = inputSize - 1; i > 0; --i) {
    buffer <<= wordSize;
    buffer += input[i - 1];
    input[i] = ((buffer >> (wordSize - 1)) & 255);
  }
  input[0] = (input[0] << 1);
}

void BasicIo::normalize(unsigned char *input, size_t inputSize,
                        BaseType correction) {
  size_t blocksInWord = sizeof(char) * 2;

  for (size_t i = 0; i < inputSize; ++i) {
    char accumulator = 0, buffer;
    for (size_t j = blocksInWord; j > 0; --j) {
      buffer = (input[i] >> ((j - 1) * 4)) & (15);
      accumulator <<= 4;
      accumulator += buffer;
      if (buffer > 4) {
        accumulator += correction;
      }
    }
    input[i] = accumulator;
  }
}

char *BasicIo::decodeToAscii(unsigned char *input, size_t inputSize) {
  char *asciiDecoded = new char[inputSize * 2 + 1];

  for (size_t i = 0; i < inputSize; ++i) {
    asciiDecoded[(inputSize - i - 1) * 2] = '0' + ((input[i] >> 4));
    asciiDecoded[(inputSize - i - 1) * 2 + 1] = '0' + (input[i] & 15);
  }
  asciiDecoded[inputSize * 2] = 0;
  return asciiDecoded;
}

void BasicIo::toDec(BaseType *input, size_t inputSize, unsigned char *output,
                    size_t outputSize) {
  size_t wordBitSize = sizeof(BaseType) * 8;
  size_t correction = (binMax - decMax) / 2;
  input[1] = 0;

  for (size_t i = inputSize; i > 0; --i) {
    BaseType buffer = input[i - 1];
    for (size_t j = wordBitSize; j > 0; --j) {
      normalize(output, outputSize, correction);
      shiftLeft(output, outputSize);
      output[0] += (buffer >> (j - 1)) & 1;
    }
  }
}
void BasicIo::toBin(const char *input, size_t inputSize, BaseType *output,
                    size_t ouptutSize) {
  for(size_t i = 0;i<inputSize;++i){
  }
}
