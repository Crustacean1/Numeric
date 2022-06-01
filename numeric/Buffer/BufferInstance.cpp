#include "BufferInstance.h"

namespace KCrypt {

BufferInstance::BufferInstance()
    : _buffers{Buffer(1), Buffer(1), Buffer(1), Buffer(1),
               Buffer(1), Buffer(1), Buffer(1)} {}

Buffer &BufferInstance::operator[](size_t bufferNo) {
  return _buffers[bufferNo];
}
BufferInstance::~BufferInstance() {}
} // namespace KCrypt
