#include <cstring>
#include "Buffer.h"

using namespace KCrypt;

BufferView::BufferView(BaseInt *newData, size_t newSize)
    : data(newData), size(newSize) {}

BufferView::BufferView(const BufferView &bufferView)
    : data(bufferView.data), size(bufferView.size) {}

BufferView &BufferView::operator=(const BufferView &bufferView) {
  data = bufferView.data;
  size = bufferView.size;
  return *this;
}

BufferView BufferView::splice(size_t newPos, size_t newSize) const {
  return BufferView(data + newPos, newSize);
}

void BufferView::copy(const BufferView & buffer) const{
  if (buffer.size < size) {
    memcpy(data, buffer.data, buffer.size * sizeof(BaseInt));
    memset(data + buffer.size, 0, (size - buffer.size) * sizeof(BaseInt));
    return;
  }
  memcpy(data, buffer.data, size * sizeof(BaseInt));
}

void BufferView::clear(char value) const{
  memset(data,value,size*sizeof(BaseInt));
}
