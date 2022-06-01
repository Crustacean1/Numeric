#include "CompEngine.h"
#include "../Utils.h"
#include <iostream>
#include <pthread.h>

namespace KCrypt {

CompEngine::CompEngine() {}

bool CompEngine::equal(const BufferView &a, const BufferView &b) const {
  BufferView::BaseInt aFill = isSigned(a) * BufferView::BaseInt(~0);
  BufferView::BaseInt bFill = isSigned(b) * BufferView::BaseInt(~0);

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

bool CompEngine::greater(const BufferView &a, const BufferView &b) const {
  BufferView::BaseInt aFill = isSigned(a) * BufferView::BaseInt(~0);
  BufferView::BaseInt bFill = isSigned(b) * BufferView::BaseInt(~0);

  if (aFill != bFill) {
    return isSigned(b);
  }

  int pos = a.size - 1;

  for (; pos > b.size - 1 && a.data[pos] == bFill; --pos) {
  }
  if (pos < 0 || a.data[pos] < bFill) {
    return false;
  }

  for (; pos >= 0 && a.data[pos] == b.data[pos]; --pos) {
  }
  return pos > 0 && a.data[pos] > b.data[pos];
}

bool CompEngine::lesser(const BufferView &a, const BufferView &b) const {
  BufferView::BaseInt aFill = isSigned(a) * BufferView::BaseInt(~0);
  BufferView::BaseInt bFill = isSigned(b) * BufferView::BaseInt(~0);

  if (aFill != bFill) {
    return isSigned(b);
  }

  int pos = a.size - 1;

  for (; pos > b.size - 1 && a.data[pos] == bFill; --pos) {
  }
  if (pos < 0 || a.data[pos] > bFill) {
    return false;
  }

  for (; pos >= 0 && a.data[pos] == b.data[pos]; --pos) {
  }
  return pos > -1 && a.data[pos] < b.data[pos];
}

bool CompEngine::greaterOrEqual(const BufferView &a,
                                const BufferView &b) const {
  BufferView::BaseInt aFill = isSigned(a) * BufferView::BaseInt(~0);
  BufferView::BaseInt bFill = isSigned(b) * BufferView::BaseInt(~0);

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

bool CompEngine::lesserOrEqual(const BufferView &a, const BufferView &b) const {
  BufferView::BaseInt aFill = isSigned(a) * BufferView::BaseInt(~0);
  BufferView::BaseInt bFill = isSigned(b) * BufferView::BaseInt(~0);

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

size_t CompEngine::leftOffset(const BufferView &a) const {
  int i, j;
  for (i = a.size - 1; i > -1 && a.data[i] == 0; --i) {
  }
  for (j = BufferView::WordSize - 1; j > -1 && ((a.data[i] >> j) & 1) == 0;
       --j) {
  }
  return (a.size - i - 1) * BufferView::WordSize +
         (BufferView::WordSize - j - 1);
}

size_t CompEngine::rightOffset(const BufferView &a) const {
  int i, j;
  for (i = 0; i < a.size && a.data[i] == 0; ++i) {
  }
  for (j = 0; j < BufferView::WordSize && ((a.data[i] >> j) & 1) == 0; ++j) {
  }
  return i * BufferView::WordSize + j;
}

size_t CompEngine::topOne(const BufferView &a) const {
  return a.size * BufferView::WordSize - 1 - leftOffset(a);
}

bool CompEngine::isSigned(const BufferView &a) const {
  return (a.data[a.size - 1] >> BufferView::WordHighBit);
}

bool CompEngine::empty(const BufferView &a) const {
  size_t i;
  for (i = 0; i < a.size && a.data[i] == 0; ++i) {
  }
  return i == a.size;
}

} // namespace KCrypt
