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

struct Buffer {
  void resize(size_t capacity);
  BufferView::BaseInt *allocate(size_t size);
  size_t computeBufferSize(size_t bufferSize);

  static constexpr size_t minSize = 4;

  BufferView::BaseInt *data;
  size_t size;
  void releaseBuffer();
  void disownBuffer();

public:
  Buffer(const Buffer &buffer) = delete;
  Buffer(Buffer &&buffer);
  Buffer(size_t newSize);
  Buffer(BufferView::BaseInt *newData, size_t newSize);
  ~Buffer();

  void reserve(size_t capacity);

  BufferView splice(size_t newPos, size_t newSize) const;
  BufferView splice() const;
  BufferView splice(size_t newSize) const;

  Buffer &operator=(const Buffer &buffer);
  Buffer &operator=(Buffer &&buffer);
};

} // namespace KCrypt

#endif /*BUFFER*/
