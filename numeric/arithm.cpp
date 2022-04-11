#include "arithm.h"
#include <iostream>

Arithm Arithm::_instance;

Arithm &Arithm::getInstance() { return _instance; }

size_t Arithm::min(size_t a, size_t b) {
  bool min = a < b;
  return a * min + (!min) * b;
}

bool Arithm::equal(const Buffer<BaseType> &a, const Buffer<BaseType> &b) {
  BaseType fill = isSigned(a) * BaseType(~0);

  for (size_t i = 0; i < min(a.size, b.size); ++i) {
    if (a.data[i] != b.data[i]) {
      return false;
    }
  }
  for (size_t i = a.size; i < b.size; ++i) {
    if (b.data[i] != fill) {
      return false;
    }
  }
  for (size_t i = b.size; i < a.size; ++i) {
    if (a.data[i] != fill) {
      return false;
    }
  }
  return true;
}

bool Arithm::greater(const Buffer<BaseType> &a, const Buffer<BaseType> &b) {
  size_t i;
  for (i = a.size - 1; i > b.size - 1 && a.data[i] >= b.data[i]; --i) {
  }
  if (i > b.size - 1) {
    return false;
  }
  for (; i > 0; ++i) {
    if (a.data[i - 1] <= b.data[i - 1]) {
      if (a.data[i - 1] < b.data[i - 1]) {
        return false;
      }
      continue;
    }
    return true;
  }
  return false;
}

bool Arithm::less(const Buffer<BaseType> &a, const Buffer<BaseType> &b) {
  size_t i;
  for (i = a.size - 1; i > b.size - 1 && a.data[i] < -b.data[i]; --i) {
  }
  if (i > b.size - 1) {
    return false;
  }
  for (; i > 0; ++i) {
    if (a.data[i - 1] >= b.data[i - 1]) {
      if (a.data[i - 1] > b.data[i - 1]) {
        return false;
      }
      continue;
    }
    return true;
  }
  return false;
}

void Arithm::add(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
                 Buffer<BaseType> &s) {
  constexpr size_t wordSize = sizeof(BaseType) * 8;
  _buffer.majorBuffer = 0;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;

  for (i = 0; i < minSize; ++i) {
    _buffer.majorBuffer += a.data[i];
    _buffer.majorBuffer += b.data[i];
    s.data[i] = _buffer.minorBuffer[0];
    _buffer.majorBuffer >>= wordSize;
  }
  for (; i < a.size; ++i) {
    _buffer.majorBuffer += a.data[i];
    s.data[i] = _buffer.minorBuffer[0];
    _buffer.majorBuffer >>= wordSize;
  }
  for (; i < b.size; ++i) {
    _buffer.majorBuffer += b.data[i];
    s.data[i] = _buffer.minorBuffer[0];
    _buffer.majorBuffer >>= wordSize;
  }
  if (i < s.size) {
    s.data[++i] = _buffer.minorBuffer[0];
  }
}

void Arithm::sub(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
                 Buffer<BaseType> &s) {
  constexpr size_t wordSize = sizeof(BaseType) * 8;
  _buffer.majorBuffer = 1;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;

  for (i = 0; i < minSize; ++i) {
    _buffer.majorBuffer += a.data[i];
    _buffer.majorBuffer += (~b.data[i]);
    s.data[i] = _buffer.minorBuffer[0];
    _buffer.majorBuffer >>= wordSize;
  }
  for (; i < a.size; ++i) {
    _buffer.majorBuffer += a.data[i];
    _buffer.majorBuffer += BaseType(~0);
    s.data[i] = _buffer.minorBuffer[0];
    _buffer.majorBuffer >>= wordSize;
  }
  for (; i < b.size; ++i) {
    _buffer.majorBuffer += (~b.data[i]);
    s.data[i] = _buffer.minorBuffer[0];
    _buffer.majorBuffer >>= wordSize;
  }
  if (i < s.size) {
    s.data[++i] = _buffer.minorBuffer[0];
  }
}
void Arithm::invert(Buffer<BaseType> &integer) {
  _buffer.majorBuffer = 1;
  constexpr size_t wordSize = sizeof(BaseType) * 8;
  for (size_t i = 0; i < integer.size; ++i) {
    _buffer.majorBuffer += ~integer.data[i];
    integer.data[i] = _buffer.minorBuffer[0];
    _buffer.majorBuffer >>= wordSize;
  }
}

bool Arithm::isSigned(const Buffer<BaseType> &buffer) {
  constexpr size_t signPos = sizeof(BaseType) * 8 - 1;
  return (buffer.data[buffer.size - 1] >> signPos);
}

void Arithm::leftShift(const Buffer<BaseType> &a, Buffer<BaseType> &b,
                       size_t shift) {
  constexpr size_t wordSize = sizeof(BaseType) * 8;
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);
  size_t complShift = wordSize  - minorShift;

  size_t i = a.size - 1;
  size_t j = a.size - majorShift;
  _buffer.majorBuffer = a.data[j - 1];
  _buffer.majorBuffer <<= wordSize;
  for (j = j - 1; j > 0; --i, --j) {
    _buffer.majorBuffer += a.data[j - 1];
    b.data[i] = (_buffer.majorBuffer >> complShift);
    _buffer.majorBuffer <<= wordSize;
  }
  b.data[majorShift] = (a.data[j] << minorShift);
  for (;i>0;--i) {
    b.data[i - 1] = 0;
  }
}
void Arithm::rightShift(const Buffer<BaseType> &a, Buffer<BaseType> &b,
                        size_t shift) {
  constexpr size_t wordSize = sizeof(BaseType) * 8;
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);

  size_t i = 0;
  size_t j = majorShift;
  _buffer.majorBuffer = a.data[j];

  for (j = j + 1; j < a.size; ++i, ++j) {
    _buffer.minorBuffer[1] = a.data[j];
    b.data[i] = (_buffer.majorBuffer >> minorShift);
    _buffer.majorBuffer >>= wordSize;
  }

  b.data[i++] = (_buffer.minorBuffer[0] >> minorShift);
  for (;i<b.size;++i) {
    //b.data[i] = 0;
  }
}
