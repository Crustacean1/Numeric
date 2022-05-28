#ifndef BUFFER
#define BUFFER

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "../Utils.h"
#include "BufferView.h"

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

template <typename T> struct Buffer : public BufferView<T> {
  void resize(size_t capacity);
  T * allocate();
  size_t computeBufferSize(size_t bufferSize);

public:
  static constexpr size_t minSize = 4;

  Buffer(const Buffer<T> &buffer);
  Buffer(size_t newSize);
  Buffer(T *newData, size_t newSize);

  void reserve(size_t capacity);
  void releaseBuffer();
  void disownBuffer();

  Buffer<T> &operator=(const Buffer<T> &buffer);
  Buffer<T> &operator=(Buffer<T> &&buffer);
};

template <typename T>
Buffer<T>::Buffer(size_t newSize)
    : BufferView<T>( nullptr,computeBufferSize(newSize)) {
  allocate();
}

template <typename T> Buffer<T> &Buffer<T>::operator=(const Buffer<T> &buffer) {
  this->data = buffer.data;
  this->size = buffer.size;
  return *this;
}

template <typename T> Buffer<T> &Buffer<T>::operator=(Buffer<T> &&buffer) {
  this->data = buffer.data;
  this->size = buffer.size;
  buffer.disownBuffer();
  return *this;
}

template <typename T>
Buffer<T>::Buffer(const Buffer<T> &buffer) : BufferView<T>(buffer) {}

template<typename T> size_t Buffer<T>::computeBufferSize(size_t bufferSize){
  size_t newSize = 1;
  while (newSize < bufferSize || newSize < minSize) {
    newSize <<= 1;
  }
  return newSize;
}
template<typename T> T * Buffer<T>::allocate(){
  return new T[this->size];
}

template <typename T> void Buffer<T>::resize(size_t capacity) {
  releaseBuffer();
  this->size = computeBufferSize(capacity);
  this->data = allocate();
}

template <typename T> void Buffer<T>::releaseBuffer() {
  if (this->data != nullptr) {
    delete[] this->data;
    this->data = nullptr;
  }
}

template <typename T> void Buffer<T>::disownBuffer() {
  this->data = nullptr;
  this->size = 0;
}

template <typename T> void Buffer<T>::reserve(size_t capacity) {
  if (this->size < capacity) {
    resize(capacity);
  }
}

#endif /*BUFFER*/
