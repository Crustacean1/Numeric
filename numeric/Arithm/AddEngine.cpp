#include "AddEngine.h"
#include "../Utils.h"
#include <iostream>

namespace KCrypt {

AddEngine::AddEngine(CompEngine &comp) : _comparator(comp) {}

void AddEngine::leftShift(const BufferView &a, const BufferView &b,
                          size_t shift) {
  size_t majorShift = shift / BufferView::WordSize;
  size_t minorShift = (shift - majorShift * BufferView::WordSize);
  size_t complShift = BufferView::WordSize - minorShift;

  size_t i = a.size - 1;
  size_t j = a.size - majorShift;
  _buffer.major = a.data[j - 1];
  _buffer.major <<= BufferView::WordSize;

  for (j = j - 1; j > 0; --i, --j) {
    _buffer.major += a.data[j - 1];
    b.data[i] = (_buffer.major >> complShift);
    _buffer.major <<= BufferView::WordSize;
  }
  b.data[majorShift] = (a.data[j] << minorShift);
  for (; i > 0; --i) {
    b.data[i - 1] = 0;
  }
}
void AddEngine::rightShift(const BufferView &a, const BufferView &b,
                           size_t shift) {
  size_t majorShift = shift / BufferView::WordSize;
  size_t minorShift = (shift - majorShift * BufferView::WordSize);

  size_t i = 0;
  size_t j = majorShift;

  _buffer.major = a.data[j];

  for (j = j + 1; j < a.size; ++i, ++j) {
    _buffer.minor.high = a.data[j];
    b.data[i] = (_buffer.major >> minorShift);
    _buffer.major >>= BufferView::WordSize;
  }
  size_t fill = _comparator.isSigned(b) * ~BufferView::BaseInt(0);

  _buffer.minor.high = fill;

  b.data[i++] = (_buffer.major >> minorShift);
  for (; i < b.size; ++i) {
    b.data[i] = fill;
  }
}

void AddEngine::addToLeft(const BufferView &a, const BufferView &b) {
  _buffer.major = 0;
  size_t i = 0;

  for (i = 0; i < b.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += b.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
  }
  BufferView::BaseInt padding =
      _comparator.isSigned(b) * BufferView::BaseInt(~0);
  for (; i < a.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += padding;
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
  }
}

void AddEngine::addToRight(const BufferView &a, const BufferView &b) {
  _buffer.major = 0;
  size_t i = 0;
  for (i = 0; i < b.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += b.data[i];
    b.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
  }
}

void AddEngine::addUnsignedToLeft(const BufferView &a, const BufferView &b) {
  _buffer.major = 0;
  size_t minSize = K::min(a.size, b.size);
  size_t i = 0;
  for (i = 0; i < minSize; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += b.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
  }
  for (; i < a.size && _buffer.major; ++i) {
    _buffer.major += a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
  }
}

void AddEngine::subFromLeft(const BufferView &a, const BufferView &b) {
  _buffer.major = 1;
  size_t i = 0;

  for (; i < b.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += (~b.data[i]);
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
  }

  BufferView::BaseInt padding =
      (b.size == 0 || !_comparator.isSigned(b)) * BufferView::BaseInt(~0);
  _buffer.major += padding;

  for (; i < a.size && _buffer.major; ++i) {
    _buffer.major += a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
    _buffer.major += padding;
  }
  _buffer.major -= padding;
}

void AddEngine::subFromRight(const BufferView &a, const BufferView &b) {
  _buffer.major = 1;
  size_t i = 0;

  for (i = 0; i < b.size; ++i) {
    _buffer.major += (~a.data[i]);
    _buffer.major += b.data[i];
    b.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
  }
}

void AddEngine::invert(const BufferView &a) {
  _buffer.major = 1;
  for (size_t i = 0; i < a.size; ++i) {
    _buffer.major += ~a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
  }
}

void AddEngine::add(const BufferView &a, BufferView::BaseInt b) {
  _buffer.major = b;

  for (size_t i = 0; _buffer.major != 0 && i < a.size; ++i) {
    _buffer.major += a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
  }
}

void AddEngine::sub(const BufferView &a, BufferView::BaseInt b) {
  _buffer.major = 1;
  _buffer.major += (~b);
  for (size_t i = 0; _buffer.major != 0 && i < a.size; ++i) {
    _buffer.major += a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= BufferView::WordSize;
    _buffer.major += BufferView::BaseInt(~0);
  }
  _buffer.major -= BufferView::BaseInt(~0);
}

bool AddEngine::overflow() { return _buffer.major & 1; }
} // namespace KCrypt
