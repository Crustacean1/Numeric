#ifndef BUFFER_INSTANCE
#define BUFFER_INSTANCE

#include "Buffer.h"

namespace KCrypt {

class BufferInstance {
  static BufferInstance *__globalInstance;

  IntBuffer _buffers[4];
  BufferInstance();

public:
  static void init();
  static BufferInstance &getInstance();
  static void destroy();

  IntBuffer &operator[](size_t bufferNo);
  ~BufferInstance();
};

} // namespace KCrypt
#endif // BUFFER_INSTANCE
