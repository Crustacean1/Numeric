#include "BufferInstance.h"

namespace KCrypt {

BufferInstance *BufferInstance::__globalInstance = nullptr;

BufferInstance::BufferInstance()
    : _buffers{Buffer(1), Buffer(1),
               Buffer(1), Buffer(1)} {}

void BufferInstance::init() { __globalInstance = new BufferInstance(); }
void BufferInstance::destroy() {
  if (__globalInstance != nullptr) {
    delete __globalInstance;
  }
}

BufferInstance &BufferInstance::getInstance() { return *__globalInstance; }

Buffer &BufferInstance::operator[](size_t bufferNo) {
  return _buffers[bufferNo];
}
BufferInstance::~BufferInstance(){
  for(int i =0;i<4;++i){
    _buffers[i].releaseBuffer();
  }
}
} // namespace KCrypt
