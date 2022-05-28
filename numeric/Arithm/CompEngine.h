#ifndef COMPARATOR
#define COMPARATOR

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt {

class CompEngine {

public:
  CompEngine();

  bool equal(const IntBufferView a, const IntBufferView b) const;
  bool greater(const IntBufferView a, const IntBufferView b) const;
  bool lesser(const IntBufferView a, const IntBufferView b) const;
  bool greaterOrEqual(const IntBufferView a, const IntBufferView b) const;
  bool lesserOrEqual(const IntBufferView a, const IntBufferView b) const;

  size_t leftOffset(const IntBufferView a) const;
  size_t rightOffset(const IntBufferView a) const;
  size_t topOne(const IntBufferView &a) const;
  bool isSigned(const IntBufferView a) const;
};

} // namespace KCrypt
#endif /*COMPARATOR*/
