#ifndef COMPARATOR
#define COMPARATOR

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt {

class CompEngine {

public:
  CompEngine();

  bool equal(const BufferView &a, const BufferView &b) const;
  bool equal(const BufferView &a, BufferView::BaseInt b) const;
  bool greater(const BufferView &a, const BufferView &b) const;
  bool lesser(const BufferView &a, const BufferView &b) const;
  bool greaterOrEqual(const BufferView &a, const BufferView &b) const;
  bool lesserOrEqual(const BufferView &a, const BufferView &b) const;

  size_t leftOffset(const BufferView &a) const;
  size_t rightOffset(const BufferView &a) const;
  size_t topOne(const BufferView &a) const;
  bool isSigned(const BufferView &a) const;
  bool empty(const BufferView & a) const;
};

} // namespace KCrypt
#endif /*COMPARATOR*/
