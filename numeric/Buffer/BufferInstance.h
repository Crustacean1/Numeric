#ifndef BUFFER_INSTANCE
#define BUFFER_INSTANCE

#include "Buffer.h"

namespace KCrypt {

class BufferInstance {
  const size_t _bufferCount;
  Buffer *const _buffers;

public:
  BufferInstance(const BufferInstance &) = delete;
  BufferInstance &operator=(const BufferInstance &) = delete;

  BufferInstance(size_t bufferCount);

  Buffer &operator[](size_t bufferNo);
  ~BufferInstance();
};

} // namespace KCrypt
#endif // BUFFER_INSTANCE
