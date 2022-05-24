#ifndef BUFFER
#define BUFFER

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <utility>

#include "Utils.h"

template <typename T> class Buffer;

using BaseInt = uint32_t;
using BufferInt = uint64_t;

using IntBuffer = Buffer<BaseInt>;

constexpr size_t wordSize = sizeof(BaseInt) * 8;
constexpr size_t bufferSize = sizeof(BufferInt) * 8;
constexpr size_t highWordPos = wordSize - 1;
constexpr size_t highBufferPos = bufferSize - 1;

union DoubleBuffer {
  BufferInt major;
  struct {
    BaseInt low;
    BaseInt high;
  } minor;
};


template <typename T> struct Buffer {
  static constexpr size_t minSize = 4;

  static Buffer<T> createBuffer(size_t bufSize);

  void reserve(size_t capacity);
  void releaseBuffer();
  void disownBuffer();
  void copy(const Buffer<T> &buffer) const;
  void clear(unsigned char value = 0) const;
  size_t resize(size_t capacity);
  Buffer<T> splice(size_t beg, size_t size) const;

  T *data = nullptr;
  size_t size = 0;
  Buffer<T> &operator=(const Buffer<T> &buffer);
  Buffer<T> &operator=(Buffer<T> &&buffer);

  Buffer(const Buffer<T> &buffer);

private:
  Buffer(size_t newSize);
  Buffer(T *newData, size_t newSize);
};

template <typename T>
Buffer<T>::Buffer(T *newData, size_t newSize) : data(newData), size(newSize) {}

template <typename T>
Buffer<T>::Buffer(size_t newSize) : size(resize(newSize)) {}

template <typename T> Buffer<T> &Buffer<T>::operator=(const Buffer<T> &buffer) {
  if (size > buffer.size) {
    memset(data + buffer.size, 0, (size - buffer.size) * sizeof(T));
  }
  memcpy(data, buffer.data, sizeof(T) * K::min(size, buffer.size));
  return *this;
}

template <typename T> Buffer<T> &Buffer<T>::operator=(Buffer<T> &&buffer) {
  data = buffer.data;
  size = buffer.size;
  buffer.disownBuffer();
  return *this;
}

template <typename T>
Buffer<T>::Buffer(const Buffer<T> &buffer)
    : data(buffer.data), size(buffer.size) {
}

template <typename T> Buffer<T> Buffer<T>::createBuffer(size_t bufSize) {
  return Buffer<T>(bufSize);
}

template <typename T> size_t Buffer<T>::resize(size_t capacity) {
  size_t newSize = 1;
  while (newSize < capacity || newSize < minSize) {
    newSize <<= 1;
  }
  releaseBuffer();
  data = new T[newSize];
  return newSize;
}

template <typename T> void Buffer<T>::releaseBuffer() {
  if (data != nullptr) {
    delete[] data;
  }
}

template <typename T> void Buffer<T>::disownBuffer() {
  data = nullptr;
  size = 0;
}

template <typename T> void Buffer<T>::clear(unsigned char value) const {
  memset(data, value, size * sizeof(T));
}

template <typename T> void Buffer<T>::reserve(size_t capacity) {
  if (size < capacity) {
    size = resize(capacity);
  }
}

template <typename T>
Buffer<T> Buffer<T>::splice(size_t beg, size_t size) const {
  return Buffer<T>{data + beg, size};
}

template <typename T> void Buffer<T>::copy(const Buffer<T> &buffer) const {
  if (buffer.size < size) {
    memcpy(data, buffer.data, buffer.size * sizeof(T));
    memset(data + buffer.size, 0, (size - buffer.size) * sizeof(T));
    return;
  }
  memcpy(data, buffer.data, size * sizeof(T));
}
#endif /*BUFFER*/
