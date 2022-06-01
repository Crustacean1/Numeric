#ifndef BUFFER_INSTANCE
#define BUFFER_INSTANCE

#include "Buffer.h"

namespace KCrypt {

class BufferInstance {
  Buffer _buffers[7];

public:
  BufferInstance();

  Buffer &operator[](size_t bufferNo);
  ~BufferInstance();
};

} // namespace KCrypt
#endif // BUFFER_INSTANCE
