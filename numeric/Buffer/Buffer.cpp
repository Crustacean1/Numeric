#include "Buffer.h"
using namespace KCrypt;

Buffer::Buffer(size_t newSize)
    : BufferView(allocate(computeBufferSize(newSize)),
                 computeBufferSize(newSize)) {}

Buffer &Buffer::operator=(const Buffer &buffer) {
  releaseBuffer();
  this->size = buffer.size;
  allocate(this->size);
  splice(0, size).copy(buffer.splice(0, buffer.size));
  return *this;
}

Buffer::Buffer(Buffer &&buffer) : BufferView(buffer.data, buffer.size) {
  buffer.disownBuffer();
}

Buffer &Buffer::operator=(Buffer &&buffer) {
  this->data = buffer.data;
  this->size = buffer.size;
  buffer.disownBuffer();
  return *this;
}

Buffer::~Buffer() { releaseBuffer(); }

size_t Buffer::computeBufferSize(size_t bufferSize) {
  size_t newSize = 1;
  while (newSize < bufferSize || newSize < minSize) {
    newSize <<= 1;
  }
  return newSize;
}

BufferView::BaseInt *Buffer::allocate(size_t newSize) {
  return new BufferView::BaseInt[newSize];
}

void Buffer::resize(size_t capacity) {
  releaseBuffer();
  this->size = computeBufferSize(capacity);
  this->data = allocate(this->size);
}

void Buffer::releaseBuffer() {
  if (this->data != nullptr) {
    delete[] this->data;
    this->data = nullptr;
  }
}

void Buffer::disownBuffer() {
  this->data = nullptr;
  this->size = 0;
}


void Buffer::reserve(size_t capacity) {
  if (this->size < capacity) {
    resize(capacity);
  }
}
