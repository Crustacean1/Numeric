#ifndef BUFFER_VIEW
#define BUFFER_VIEW

#include <cstddef>
#include <cstdint>

namespace KCrypt {

struct BufferView {

public:
  using BaseInt = uint32_t;
  using BufferInt = uint64_t;
  static constexpr size_t WordSize = sizeof(BaseInt) * 8;
  static constexpr size_t BufferSize = sizeof(BufferInt) * 8;
  static constexpr size_t WordHighBit = WordSize - 1;
  static constexpr size_t BufferHighBit = BufferSize - 1;

  BaseInt *data;
  size_t size;

  BufferView(BaseInt *newData, size_t newSize);
  BufferView(const BufferView &bufferView);

  void copy(const BufferView &buffer) const;
  void clear(char value = 0) const;

  BufferView splice() const;
  BufferView splice(size_t newSize) const;
  BufferView splice(size_t newPos, size_t newSize) const;

  BufferView &operator=(const BufferView &bufferView);

};

} // namespace KCrypt
#endif /*BUFFER_VIEW*/
