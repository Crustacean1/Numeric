#include "AddEngine.h"
#include "../Utils.h"
#include <iostream>

namespace KCrypt {

AddEngine::AddEngine(CompEngine &comp) : _comparator(comp) {}

void AddEngine::leftShift(const IntBufferView &a, const IntBufferView &b,
                          size_t shift) {
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);
  size_t complShift = wordSize - minorShift;

  size_t i = a.size - 1;
  size_t j = a.size - majorShift;
  _buffer.major = a.data[j - 1];
  _buffer.major <<= wordSize;
  for (j = j - 1; j > 0; --i, --j) {
    _buffer.major += a.data[j - 1];
    b.data[i] = (_buffer.major >> complShift);
    _buffer.major <<= wordSize;
  }
  b.data[majorShift] = (a.data[j] << minorShift);
  for (; i > 0; --i) {
    b.data[i - 1] = 0;
  }
}
void AddEngine::rightShift(const IntBufferView &a, const IntBufferView &b,
                           size_t shift) {
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);

  size_t i = 0;
  size_t j = majorShift;
  _buffer.major = a.data[j];

  for (j = j + 1; j < a.size; ++i, ++j) {
    _buffer.minor.high = a.data[j];
    b.data[i] = (_buffer.major >> minorShift);
    _buffer.major >>= wordSize;
  }

  b.data[i++] = (_buffer.minor.low >> minorShift);
  for (; i < b.size; ++i) {
    b.data[i] = 0;
  }
}

void AddEngine::addToLeft(const IntBufferView a, const IntBufferView b) {
  _buffer.major = 0;
  size_t i = 0;

  for (i = 0; i < b.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += b.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
  BaseInt padding = _comparator.isSigned(b) * BaseInt(~0);
  for (; i < a.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += padding;
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

void AddEngine::addToRight(const IntBufferView a, const IntBufferView b) {
  _buffer.major = 0;
  size_t i = 0;
  for (i = 0; i < b.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += b.data[i];
    b.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

void AddEngine::addUnsignedToLeft(const IntBufferView a, const IntBufferView b) {
  _buffer.major = 0;
  size_t minSize = K::min(a.size, b.size);
  size_t i = 0;
  for (i = 0; i < minSize; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += b.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
  for (; i < a.size && _buffer.major; ++i) {
    _buffer.major += a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

void AddEngine::subFromLeft(const IntBufferView a, const IntBufferView b) {
  _buffer.major = 1;
  size_t i = 0;

  for (; i < b.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += (~b.data[i]);
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }

  BaseInt padding = (b.size == 0 || !_comparator.isSigned(b)) * BaseInt(~0);
  _buffer.major += padding;

  for (; i < a.size && _buffer.major; ++i) {
    _buffer.major += a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
    _buffer.major += padding;
  }
  _buffer.major -= padding;
}

void AddEngine::subFromRight(const IntBufferView a, const IntBufferView b) {
  _buffer.major = 1;
  size_t i = 0;

  for (i = 0; i < b.size; ++i) {
    _buffer.major += (~a.data[i]);
    _buffer.major += b.data[i];
    b.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

void AddEngine::invert(const IntBufferView a) {
  _buffer.major = 1;
  for (size_t i = 0; i < a.size; ++i) {
    _buffer.major += ~a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

void AddEngine::add(IntBufferView a, BaseInt b) {
  _buffer.major = b;

  for (size_t i = 0; _buffer.major != 0 && i < a.size; ++i) {
    _buffer.major += a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

bool AddEngine::overflow() { return _buffer.major & 1; }
} // namespace KCrypt
