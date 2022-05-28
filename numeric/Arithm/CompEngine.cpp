#include "CompEngine.h"
#include "../Utils.h"

namespace KCrypt {

CompEngine::CompEngine() {}

bool CompEngine::equal(const IntBufferView a, const IntBufferView b) const {
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

bool CompEngine::greater(const IntBufferView a, const IntBufferView b) const {
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

bool CompEngine::lesser(const IntBufferView a, const IntBufferView b) const {
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

bool CompEngine::greaterOrEqual(const IntBufferView a, const IntBufferView b) const {
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

bool CompEngine::lesserOrEqual(const IntBufferView a, const IntBufferView b) const {
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

size_t CompEngine::leftOffset(const IntBufferView a) const {
  int i, j;
  for (i = a.size - 1; i > -1 && a.data[i] == 0; --i) {
  }
  for (j = wordSize - 1; j > -1 && ((a.data[i] >> j) & 1) == 0; --j) {
  }
  return (a.size - i - 1) * wordSize + (wordSize - j - 1);
}

size_t CompEngine::rightOffset(const IntBufferView a) const {
  int i, j;
  for (i = 0; i < a.size && a.data[i] == 0; ++i) {
  }
  for (j = 0; j < wordSize && ((a.data[i] >> j) & 1) == 0; ++j) {
  }
  return i * wordSize + j;
}

size_t CompEngine::topOne(const IntBufferView &a) const {
  return a.size * wordSize - 1 - leftOffset(a);
}

bool CompEngine::isSigned(const IntBufferView a) const {
  return (a.data[a.size - 1] >> highWordPos);
}

} // namespace KCrypt
