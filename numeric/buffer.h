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
  static Buffer<T> reserve(Buffer<T> &buffer, size_t capacity);

  void releaseBuffer();
  void disownBuffer();
  void clear(unsigned char value = 0);

  T *data = nullptr;
  size_t size = 0;
  Buffer<T> &operator=(const Buffer<T> &buffer);
  Buffer<T> &operator=(Buffer<T> &&buffer);

  Buffer(const Buffer<T> &buffer);
  Buffer(Buffer<T> &&buffer);

private:
  Buffer(T *newData, size_t newSize);
};

template <typename T>
Buffer<T>::Buffer(T *newData, size_t newSize) : data(newData), size(newSize) {}

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
  size_t newSize = 1;
  while (newSize < bufSize || newSize < minSize) {
    newSize <<= 1;
  }
  auto data = new T[newSize];
  return Buffer<BaseType>(data, newSize);
}

template <typename T> void Buffer<T>::releaseBuffer() {
  if (data != nullptr) {
    delete[] data;
  }
}

template <typename T> void Buffer<T>::disownBuffer() { data = nullptr; }

template <typename T> void Buffer<T>::clear(unsigned char value) {
  memset(data, value, size * sizeof(T));
}

template <typename T>
Buffer<T> Buffer<T>::reserve(Buffer<T> &buffer, size_t capacity) {
  if (buffer.size < capacity) {
    return createBuffer(capacity);
  }
  return buffer;
}
#endif /*BUFFER*/
