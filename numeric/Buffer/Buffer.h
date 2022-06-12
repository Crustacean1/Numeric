#ifndef BUFFER
#define BUFFER

#include "BufferView.h"
#include "../Utils.h"

namespace KCrypt {

union DoubleBuffer {
  BufferView::BufferInt major;
  struct {
    BufferView::BaseInt low;
    BufferView::BaseInt high;
  } minor;
};

struct Buffer : public BufferView{
  void resize(size_t capacity);
  BufferView::BaseInt *allocate(size_t size);
  size_t computeBufferSize(size_t bufferSize);

  static constexpr size_t minSize = 4;

  void releaseBuffer();
  void disownBuffer();

public:
  Buffer(const Buffer &buffer) = delete;
  Buffer(Buffer &&buffer);
  Buffer(size_t newSize = 1);
  Buffer(BufferView::BaseInt *newData, size_t newSize);
  ~Buffer();

  void reserve(size_t capacity);

  Buffer &operator=(const Buffer &buffer);
  Buffer &operator=(Buffer &&buffer);
};

} // namespace KCrypt

#endif /*BUFFER*/
