#ifndef COMPARATOR
#define COMPARATOR

#include "../Buffer.h"

namespace KCrypt {

class Comparator {

public:
  Comparator();

  bool equal(const IntBuffer a, const IntBuffer b) const;
  bool greater(const IntBuffer a, const IntBuffer b) const;
  bool lesser(const IntBuffer a, const IntBuffer b) const;
  bool greaterOrEqual(const IntBuffer a, const IntBuffer b) const;
  bool lesserOrEqual(const IntBuffer a, const IntBuffer b) const;

  size_t leftOffset(const IntBuffer a) const;
  size_t rightOffset(const IntBuffer a) const;
  size_t topOne(const IntBuffer &a) const;
  bool isSigned(const IntBuffer a) const;
};

} // namespace KCrypt
#endif /*COMPARATOR*/
