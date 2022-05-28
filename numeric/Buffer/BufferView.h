#ifndef BUFFER_VIEW
#define BUFFER_VIEW

#include <cstddef>
#include <cstdint>
#include <cstring>

template <typename T> struct BufferView {
  BufferView(T *newData, size_t newSize);

public:
  size_t size;
  T *data;
  BufferView(const BufferView<T> &bufferView);

  BufferView splice(size_t pos, size_t size) const;
  void copy(const BufferView<T> &buffer) const;
  void clear(char value = 0) const;

  BufferView &operator=(const BufferView<T> &bufferView);
};

using IntBufferView = BufferView<uint32_t>;

template <typename T>
BufferView<T>::BufferView(T *newData, size_t newSize)
    : data(newData), size(newSize) {}

template <typename T>
BufferView<T>::BufferView(const BufferView<T> &bufferView)
    : data(bufferView.data), size(bufferView.size) {}

template <typename T>
BufferView<T> &BufferView<T>::operator=(const BufferView<T> &bufferView) {
  data = bufferView.data;
  size = bufferView.size;
}

template <typename T>
BufferView<T> BufferView<T>::splice(size_t beg, size_t size) const {
  return BufferView<T>(data + beg, size);
}

template <typename T>
void BufferView<T>::copy(const BufferView<T> &buffer) const {
  // TODO: Branchless version
  if (buffer.size < size) {
    memcpy(data, buffer.data, buffer.size * sizeof(T));
    memset(data + buffer.size, 0, (size - buffer.size) * sizeof(T));
    return;
  }
  memcpy(data, buffer.data, size * sizeof(T));
}

template <typename T> void BufferView<T>::clear(char value) const {
  memset(data, value, size * sizeof(T));
}

#endif /*BUFFER_VIEW*/
