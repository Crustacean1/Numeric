#include "arithm.h"
#include <iostream>

Arithm Arithm::_instance;

Arithm &Arithm::getInstance() { return _instance; }

Arithm::Arithm()
    : _aBuffer(BaseBuffer::createBuffer(1)),
      _bBuffer(BaseBuffer::createBuffer(1)) {}

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

size_t Arithm::leftOffset(SourceBuffer a) {
  int i, j;
  for (i = a.size - 1; i > -1 && a.data[i] == 0; --i) {
  }
  for (j = wordSize - 1; j > -1 && ((a.data[i] >> j) & 1) == 0; --j) {
  }
  return (a.size - i - 1) * wordSize + (wordSize - j - 1);
}

size_t Arithm::rightOffset(SourceBuffer a) {
  int i, j;
  for (i = 0; i < a.size && a.data[i] == 0; ++i) {
  }
  for (j = 0; j < wordSize && ((a.data[i] >> j) & 1) == 0; ++j) {
  }
  return (i - 1) * wordSize + (j - 1);
}

void Arithm::add(SourceBuffer a, SourceBuffer b, OutputBuffer s) {
  _wordBuffer.major = 0;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;

  for (i = 0; i < minSize; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += b.data[i];
    s.data[i] = _wordBuffer.minor[0];
    _wordBuffer.major >>= wordSize;
  }
  for (; i < a.size; ++i) {
    _wordBuffer.major += a.data[i];
    s.data[i] = _wordBuffer.minor[0];
    _wordBuffer.major >>= wordSize;
  }
  for (; i < b.size; ++i) {
    _wordBuffer.major += b.data[i];
    s.data[i] = _wordBuffer.minor[0];
    _wordBuffer.major >>= wordSize;
  }
  if (i < s.size) {
    s.data[++i] = _wordBuffer.minor[0];
  }
}

void Arithm::sub(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
                 Buffer<BaseType> &s) {
  _wordBuffer.major = 1;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;

  for (i = 0; i < minSize; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += (~b.data[i]);
    s.data[i] = _wordBuffer.minor[0];
    _wordBuffer.major >>= wordSize;
  }
  for (; i < a.size; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += BaseType(~0);
    s.data[i] = _wordBuffer.minor[0];
    _wordBuffer.major >>= wordSize;
  }
  for (; i < b.size; ++i) {
    _wordBuffer.major += (~b.data[i]);
    s.data[i] = _wordBuffer.minor[0];
    _wordBuffer.major >>= wordSize;
  }
  if (i < s.size) {
    s.data[++i] = _wordBuffer.minor[0];
  }
}
void Arithm::invert(Buffer<BaseType> &integer) {
  _wordBuffer.major = 1;
  for (size_t i = 0; i < integer.size; ++i) {
    _wordBuffer.major += ~integer.data[i];
    integer.data[i] = _wordBuffer.minor[0];
    _wordBuffer.major >>= wordSize;
  }
}

bool Arithm::isSigned(const Buffer<BaseType> &buffer) {
  constexpr size_t signPos = sizeof(BaseType) * 8 - 1;
  return (buffer.data[buffer.size - 1] >> signPos);
}

void Arithm::leftShift(const Buffer<BaseType> &a, Buffer<BaseType> &b,
                       size_t shift) {
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);
  size_t complShift = wordSize - minorShift;

  size_t i = a.size - 1;
  size_t j = a.size - majorShift;
  _wordBuffer.major = a.data[j - 1];
  _wordBuffer.major <<= wordSize;
  for (j = j - 1; j > 0; --i, --j) {
    _wordBuffer.major += a.data[j - 1];
    b.data[i] = (_wordBuffer.major >> complShift);
    _wordBuffer.major <<= wordSize;
  }
  b.data[majorShift] = (a.data[j] << minorShift);
  for (; i > 0; --i) {
    b.data[i - 1] = 0;
  }
}
void Arithm::rightShift(const Buffer<BaseType> &a, Buffer<BaseType> &b,
                        size_t shift) {
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);

  size_t i = 0;
  size_t j = majorShift;
  _wordBuffer.major = a.data[j];

  for (j = j + 1; j < a.size; ++i, ++j) {
    _wordBuffer.minor[1] = a.data[j];
    b.data[i] = (_wordBuffer.major >> minorShift);
    _wordBuffer.major >>= wordSize;
  }

  b.data[i++] = (_wordBuffer.minor[0] >> minorShift);
  for (; i < b.size; ++i) {
    b.data[i] = 0;
  }
}

void Arithm::mul(SourceBuffer a, SourceBuffer b, OutputBuffer c) {
  _wordBuffer.major = 0;
  BufferType mulCache;
  for (size_t i = 0; i < c.size; ++i) {
    for (size_t j = (i > b.size) * (i - 1 - b.size); j < a.size && j <= i;
         ++j) {
      mulCache = a.data[j];
      mulCache *= b.data[i - j];
      _wordBuffer.major += mulCache;
    }
    c.data[i] = _wordBuffer.minor[0];
    _wordBuffer.major >>= wordSize;
  }
}

void Arithm::div(SourceBuffer a, SourceBuffer b, OutputBuffer c) {
  size_t shift = leftOffset(b) + 1;
  shift -= leftOffset(a);

  if (((int)shift) < 1) {
    return;
  }

  _aBuffer = BaseBuffer::reserve(_aBuffer, a.size);
  _bBuffer = BaseBuffer::reserve(_bBuffer, a.size);
  _aBuffer = a;
  _bBuffer = b;
  c.clear();

  leftShift(_bBuffer, _bBuffer, shift);

  for (size_t i = 0; i < shift + 1; ++i) {
    leftShift(c, c, 1);
    rightShift(_bBuffer, _bBuffer, 1);
    // std::cout << "a: " << _aBuffer.data[0] << std::endl;
    if (isSigned(_aBuffer)) {
      // std::cout << "add: " << _bBuffer.data[0] << std::endl;
      add(_aBuffer, _bBuffer, _aBuffer);
      continue;
    }
    // std::cout << "sub: " << _bBuffer.data[0] << std::endl;
    sub(_aBuffer, _bBuffer, _aBuffer);
    c.data[0] += 1;
  }
  _wordBuffer.minor[0] = wordSize - ((shift) % wordSize);
  (c.data[shift / wordSize] <<= _wordBuffer.minor[0]) >>= _wordBuffer.minor[0];
}
