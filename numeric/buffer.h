#ifndef BUFFER
#define BUFFER

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility>

#include "utils.h"

using BaseType = uint32_t;
using BufferType = uint64_t;
constexpr size_t BaseWordSize = sizeof(BaseType) * 8;

template <typename T> struct Buffer {
  static constexpr size_t minSize = 4;
  static constexpr size_t blockSize = 2;

  static Buffer<T> createBuffer(size_t bufSize);
  static void releaseBuffer(Buffer<T> &buffer);
  static void disownBuffer(Buffer<T> &buffer);
  static void clear(Buffer<T> &buffer,unsigned char value = 0);

  T *data = nullptr;
  const size_t size = 0;
  Buffer<T> &operator=(const Buffer<T> &buffer);
  Buffer<T> &operator=(Buffer<T> &&buffer);

  Buffer(const Buffer<T> &buffer);
  Buffer(Buffer<T> &&buffer);

private:
  Buffer(T *newData, size_t newSize);
};

template <typename T>
Buffer<T>::Buffer(T *newData, size_t newSize) : data(newData), size(newSize) {}

template <typename T> Buffer<T> Buffer<T>::createBuffer(size_t bufSize) {
  size_t newSize = 1;
  while (newSize < bufSize || newSize < minSize) {
    newSize <<= blockSize;
  }
  auto data = new T[newSize];
  return Buffer<BaseType>(data, newSize);
}

template <typename T> void Buffer<T>::releaseBuffer(Buffer<T> &buffer) {
  if (buffer.data != nullptr) {
    delete[] buffer.data;
  }
}

template <typename T> void Buffer<T>::disownBuffer(Buffer<T> &buffer) {
  buffer.data = nullptr;
}

template <typename T> Buffer<T> &Buffer<T>::operator=(const Buffer<T> &buffer) {
  if (size > buffer.size) {
    memset(data + buffer.size, 0, (size - buffer.size) * sizeof(T));
  }
  memcpy(data, buffer.data, sizeof(T) * KUtils::min(size, buffer.size));
  return *this;
}
template <typename T> Buffer<T> &Buffer<T>::operator=(Buffer<T> &&buffer) {
  if (size == buffer.size) {
    releaseBuffer(*this);
    data = buffer.data;
    disownBuffer(buffer);
    return *this;
  }
  (*this) = buffer;
  disownBuffer(buffer);
  return *this;
}

template <typename T>
Buffer<T>::Buffer(const Buffer<T> &buffer)
    : data(new T[buffer.size]), size(buffer.size) {
  memcpy(data, buffer.data, size * sizeof(T));
}

template <typename T>
Buffer<T>::Buffer(Buffer<T> &&buffer) : data(buffer.data), size(buffer.size) {
  disownBuffer(buffer);
}

template <typename T> void Buffer<T>::clear(Buffer<T> &buffer,unsigned char value) {
  memset(buffer.data, value, buffer.size * sizeof(T));
}

#endif /*BUFFER*/
