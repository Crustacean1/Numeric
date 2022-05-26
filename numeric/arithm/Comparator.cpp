#include "Comparator.h"
#include "../Utils.h"

namespace KCrypt {

Comparator::Comparator() {}

bool Comparator::equal(const IntBuffer a, const IntBuffer b) const {
  BaseInt aFill = isSigned(a) * BaseInt(~0);
  BaseInt bFill = isSigned(b) * BaseInt(~0);

  if (aFill != bFill) {
    // return false;
  }

  int pos = K::max(a.size - 1, b.size - 1);

  for (; pos > a.size - 1 && b.data[pos] == aFill; --pos) {
  }
  if (pos != a.size - 1) {
    return false;
  }

  for (; pos > b.size - 1 && a.data[pos] == bFill; --pos) {
  }
  if (pos != b.size - 1) {
    return false;
  }

  for (; pos >= 0 && a.data[pos] == b.data[pos]; --pos) {
  }
  return pos == -1;
}

bool Comparator::greater(const IntBuffer a, const IntBuffer b) const {
  BaseInt aFill = isSigned(a) * BaseInt(~0);
  BaseInt bFill = isSigned(b) * BaseInt(~0);

  if (aFill != bFill) {
    return isSigned(b);
  }

  int pos = K::max(a.size - 1, b.size - 1);

  for (; pos > a.size - 1 && b.data[pos] <= aFill; --pos) {
  }
  if (b.data[pos] > aFill) {
    return false;
  }

  for (; pos > b.size - 1 && a.data[pos] >= bFill; --pos) {
  }
  if (a.data[pos] < bFill) {
    return false;
  }

  for (; pos >= 0 && a.data[pos] >= b.data[pos]; --pos) {
  }
  return pos == -1 && a.data[0] > b.data[0];
}

bool Comparator::lesser(const IntBuffer a, const IntBuffer b) const {
  BaseInt aFill = isSigned(a) * BaseInt(~0);
  BaseInt bFill = isSigned(b) * BaseInt(~0);

  if (aFill != bFill) {
    return isSigned(b);
  }

  int pos = K::max(a.size - 1, b.size - 1);

  for (; pos > a.size - 1 && b.data[pos] >= aFill; --pos) {
  }
  if (b.data[pos] < aFill) {
    return false;
  }

  for (; pos > b.size - 1 && a.data[pos] <= bFill; --pos) {
  }
  if (a.data[pos] > bFill) {
    return false;
  }

  for (; pos >= 0 && a.data[pos] >= b.data[pos]; --pos) {
  }
  return pos == -1 && a.data[0] < b.data[0];
}

bool Comparator::greaterOrEqual(const IntBuffer a, const IntBuffer b) const {
  BaseInt aFill = isSigned(a) * BaseInt(~0);
  BaseInt bFill = isSigned(b) * BaseInt(~0);

  if (aFill != bFill) {
    return isSigned(b);
  }

  int pos = K::max(a.size - 1, b.size - 1);

  for (; pos > a.size - 1 && b.data[pos] <= aFill; --pos) {
  }
  if (b.data[pos] > aFill) {
    return false;
  }

  for (; pos > b.size - 1 && a.data[pos] >= bFill; --pos) {
  }
  if (a.data[pos] < bFill) {
    return false;
  }

  for (; pos >= 0 && a.data[pos] >= b.data[pos]; --pos) {
  }
  return pos == -1;
}

bool Comparator::lesserOrEqual(const IntBuffer a, const IntBuffer b) const {
  BaseInt aFill = isSigned(a) * BaseInt(~0);
  BaseInt bFill = isSigned(b) * BaseInt(~0);

  if (aFill != bFill) {
    return isSigned(b);
  }

  int pos = K::max(a.size - 1, b.size - 1);

  for (; pos > a.size - 1 && b.data[pos] >= aFill; --pos) {
  }
  if (b.data[pos] < aFill) {
    return false;
  }

  for (; pos > b.size - 1 && a.data[pos] <= bFill; --pos) {
  }
  if (a.data[pos] > bFill) {
    return false;
  }

  for (; pos >= 0 && a.data[pos] >= b.data[pos]; --pos) {
  }
  return pos == -1;
}

size_t Comparator::leftOffset(const IntBuffer a) const {
  int i, j;
  for (i = a.size - 1; i > -1 && a.data[i] == 0; --i) {
  }
  for (j = wordSize - 1; j > -1 && ((a.data[i] >> j) & 1) == 0; --j) {
  }
  return (a.size - i - 1) * wordSize + (wordSize - j - 1);
}

size_t Comparator::rightOffset(const IntBuffer a) const {
  int i, j;
  for (i = 0; i < a.size && a.data[i] == 0; ++i) {
  }
  for (j = 0; j < wordSize && ((a.data[i] >> j) & 1) == 0; ++j) {
  }
  return i * wordSize + j;
}

size_t Comparator::topOne(const IntBuffer &a) const {
  return a.size * wordSize - 1 - leftOffset(a);
}

bool Comparator::isSigned(const IntBuffer a) const {
  return (a.data[a.size - 1] >> highWordPos);
}

} // namespace KCrypt
