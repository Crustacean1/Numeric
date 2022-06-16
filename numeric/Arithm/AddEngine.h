#ifndef ADD_ENGINE
#define ADD_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"
#include "CompEngine.h"

namespace KCrypt {
class ArithmInjector;

class AddEngine {

  DoubleBuffer _buffer;
  CompEngine &_comparator;

public:
  AddEngine(ArithmInjector &arithm);

  void leftShift(const BufferView &source, const BufferView &output,
                 size_t offset);
  void rightShift(const BufferView &source, const BufferView &output,
                  size_t offset);

  // All arguments must be ordered by descending size from left
  void addToLeft(const BufferView &a, const BufferView &b);
  void addToRight(const BufferView &a, const BufferView &b);
  void addUnsignedToLeft(const BufferView &a, const BufferView &b);
  void add(const BufferView &a, BufferView::BaseInt b);

  void subFromLeft(const BufferView &a, const BufferView &b);
  void subFromRight(const BufferView &a, const BufferView &b);
  void sub(const BufferView &a, BufferView::BaseInt b);
  void invert(const BufferView &a);

  bool overflow();
};

} // namespace KCrypt
#endif /*ADD_ENGINE*/
