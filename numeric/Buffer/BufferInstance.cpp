#include "BufferInstance.h"
#include <iostream>

namespace KCrypt {

BufferInstance::BufferInstance(size_t bufferCount)
    : _bufferCount(bufferCount), _buffers(new Buffer[_bufferCount]) {}

Buffer &BufferInstance::operator[](size_t bufferNo) {
  return _buffers[bufferNo];
}
BufferInstance::~BufferInstance() { delete[] _buffers; }
} // namespace KCrypt
