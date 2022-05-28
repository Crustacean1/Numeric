#ifndef ADD_ENGINE
#define ADD_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"
#include "CompEngine.h"

namespace KCrypt {

class AddEngine {

  DoubleBuffer _buffer;
  CompEngine &_comparator;

public:
  AddEngine(CompEngine &comp);

  void leftShift(const IntBufferView &source, const IntBufferView &output,
                 size_t offset);
  void rightShift(const IntBufferView &source, const IntBufferView &output,
                  size_t offset);

  // All arguments must be ordered by descending size from left
  void addToLeft(const IntBufferView a, const IntBufferView b);
  void addToRight(const IntBufferView a, const IntBufferView b);
  void addUnsignedToLeft(const IntBufferView a, const IntBufferView b);
  void add(const IntBufferView a, BaseInt b);

  void subFromLeft(const IntBufferView a, const IntBufferView b);
  void subFromRight(const IntBufferView a, const IntBufferView b);
  void invert(const IntBufferView a);

  bool overflow();
};

} // namespace KCrypt
#endif /*ADD_ENGINE*/
