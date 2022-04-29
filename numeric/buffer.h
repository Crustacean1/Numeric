#ifndef BUFFER
#define BUFFER

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <utility>

#include "utils.h"

using BaseType = uint32_t;
using BufferType = uint64_t;
constexpr size_t BaseWordSize = sizeof(BaseType) * 8;

template <typename T> struct Buffer {
  static constexpr size_t minSize = 4;

  static Buffer<T> createBuffer(size_t bufSize);
  static void reserve(Buffer<T> &buffer, size_t capacity);

  void releaseBuffer();
  void disownBuffer();
  void clear(unsigned char value = 0) const;
  size_t resize(size_t capacity);
  Buffer<T> splice(size_t beg, size_t size) const;

  T *data = nullptr;
  size_t size = 0;
  Buffer<T> &operator=(const Buffer<T> &buffer);
  Buffer<T> &operator=(Buffer<T> &&buffer);

  Buffer(const Buffer<T> &buffer);
  Buffer(Buffer<T> &&buffer);

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
  memcpy(data, buffer.data, sizeof(T) * KUtils::min(size, buffer.size));
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
    : data(new T[buffer.size]), size(buffer.size) {
  memcpy(data, buffer.data, size * sizeof(T));
}

template <typename T>
Buffer<T>::Buffer(Buffer<T> &&buffer) : data(buffer.data), size(buffer.size) {
  buffer.disownBuffer();
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

template <typename T> void Buffer<T>::disownBuffer() { data = nullptr; }

template <typename T> void Buffer<T>::clear(unsigned char value) const{
  memset(data, value, size * sizeof(T));
}

template <typename T>
void Buffer<T>::reserve(Buffer<T> &buffer, size_t capacity) {
  if (buffer.size < capacity) {
    buffer.size = buffer.resize(capacity);
  }
}
template <typename T>
Buffer<T> Buffer<T>::splice(size_t beg, size_t size) const {
  return Buffer<T>{data + beg, size};
}
#endif /*BUFFER*/
