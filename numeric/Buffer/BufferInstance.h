#ifndef BUFFER_INSTANCE
#define BUFFER_INSTANCE

#include "Buffer.h"

namespace KCrypt {

class BufferInstance {
  static BufferInstance *__globalInstance;

  Buffer _buffers[7];
  BufferInstance();

public:
  static void init();
  static BufferInstance &getInstance();
  static void destroy();

  Buffer &operator[](size_t bufferNo);
  ~BufferInstance();
};

} // namespace KCrypt
#endif // BUFFER_INSTANCE
