#define __DEBUG(depth, x)                                                      \
  if (depth == 1) {                                                            \
    for (int dpth = 0; dpth < depth; ++dpth) {                                 \
      std::cout << " ";                                                        \
    }                                                                          \
    std::cout << "at: " << __LINE__ << ",\t" << #x << "\t=\t" << x.size        \
              << ": " << io.getDec(x, *this) <<" "<<isSigned(x)<< std::endl;                     \
  }
#include "arithm.h"
#include "basicio.h"
#include <iostream>

Arithm Arithm::_instance;

Arithm &Arithm::getInstance() { return _instance; }

Arithm::Arithm()
    : _aBuffer(BaseBuffer::createBuffer(1)),
      _bBuffer(BaseBuffer::createBuffer(1)) {}

bool Arithm::overflow() { return _wordBuffer.major & 1; }

size_t Arithm::min(size_t a, size_t b) {
  bool min = a < b;
  return a * min + (!min) * b;
}

bool Arithm::equal(SourceBuffer a, SourceBuffer b) {
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

bool Arithm::greater(SourceBuffer a, SourceBuffer b) {
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

bool Arithm::less(SourceBuffer a, SourceBuffer b) {
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

void Arithm::add(SourceBuffer a, SourceBuffer b, SourceBuffer s) {
  _wordBuffer.major = 0;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;

  for (i = 0; i < minSize; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += b.data[i];
    s.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  for (; i < a.size; ++i) {
    _wordBuffer.major += a.data[i];
    s.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  for (; i < b.size; ++i) {
    _wordBuffer.major += b.data[i];
    s.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  if (i < s.size) {
    s.data[++i] = _wordBuffer.minor.low;
  }
}
void Arithm::unsigned_add(SourceBuffer a, SourceBuffer b) {
  _wordBuffer.major = 0;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;
  for (i = 0; i < minSize; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += b.data[i];
    a.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  for (; i < a.size && _wordBuffer.major; ++i) {
    _wordBuffer.major += a.data[i];
    a.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}

void Arithm::sub(SourceBuffer a, SourceBuffer b, SourceBuffer s) {
  _wordBuffer.major = 1;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;

  for (i = 0; i < minSize; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += (~b.data[i]);
    s.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  for (; i < a.size; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += BaseType(~0);
    s.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  for (; i < b.size; ++i) {
    _wordBuffer.major += (~b.data[i]);
    s.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  if (i < s.size) {
    s.data[++i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}
void Arithm::invert(SourceBuffer integer) {
  _wordBuffer.major = 1;
  for (size_t i = 0; i < integer.size; ++i) {
    _wordBuffer.major += ~integer.data[i];
    integer.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}

bool Arithm::isSigned(SourceBuffer buffer) {
  constexpr size_t signPos = sizeof(BaseType) * 8 - 1;
  return (buffer.data[buffer.size - 1] >> signPos);
}

void Arithm::leftShift(SourceBuffer a, SourceBuffer b, size_t shift) {
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
void Arithm::rightShift(SourceBuffer a, SourceBuffer b, size_t shift) {
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);

  size_t i = 0;
  size_t j = majorShift;
  _wordBuffer.major = a.data[j];

  for (j = j + 1; j < a.size; ++i, ++j) {
    _wordBuffer.minor.high = a.data[j];
    b.data[i] = (_wordBuffer.major >> minorShift);
    _wordBuffer.major >>= wordSize;
  }

  b.data[i++] = (_wordBuffer.minor.low >> minorShift);
  for (; i < b.size; ++i) {
    b.data[i] = 0;
  }
}

void Arithm::mul(SourceBuffer b, SourceBuffer a) {
  BufferType mulCache;
  for (int i = a.size - 1, j = 0; i >= 0; --i) {
    mulCache = a.data[i];
    _wordBuffer.major = 0;
    a.data[i] = 0;
    for (j = 0; j + i < a.size & j < b.size; ++j) {
      _wordBuffer.major += a.data[i + j];
      _wordBuffer.major += mulCache * b.data[j];
      a.data[i + j] = _wordBuffer.major;
      _wordBuffer.major >>= wordSize;
    }
    for (; j + i < a.size && _wordBuffer.major != 0; ++j) {
      _wordBuffer.major += a.data[i + j];
      a.data[i + j] = _wordBuffer.major;
      _wordBuffer.major >>= wordSize;
    }
  }
}

void Arithm::div(SourceBuffer a, SourceBuffer b, SourceBuffer c) {
  size_t aShift = a.size * wordSize - leftOffset(a);
  size_t bShift = b.size * wordSize - leftOffset(b);
  size_t shift = aShift - bShift;

  if (((int)shift) < 0) {
    return;
  }

  BaseBuffer::reserve(_aBuffer, a.size);
  BaseBuffer::reserve(_bBuffer, a.size);

  _aBuffer = a;
  _bBuffer = b;

  c.clear();

  leftShift(_bBuffer, _bBuffer, shift);

  for (size_t i = 0; i < shift + 2; ++i) {
    leftShift(c, c, 1);
    if (isSigned(_aBuffer)) {
      add(_aBuffer, _bBuffer, _aBuffer);
    } else {
      sub(_aBuffer, _bBuffer, _aBuffer);
      c.data[0] += 1;
    }
    rightShift(_bBuffer, _bBuffer, 1);
  }

  _wordBuffer.minor.low = wordSize - ((shift + 1) % wordSize);

  (c.data[(shift + 1) / wordSize] <<= _wordBuffer.minor.low) >>=
      _wordBuffer.minor.low;
  c.data[(shift + 1) / wordSize] *= (_wordBuffer.minor.low != 32);
}

void Arithm::kar(SourceBuffer a, SourceBuffer b, SourceBuffer c) {
  Buffer<BaseType>::reserve(_aBuffer, 4 * a.size);
  karIt(a, b, c, _aBuffer);
}
void Arithm::karIt(SourceBuffer a, SourceBuffer b, SourceBuffer c,
                   SourceBuffer d, int level) {

  BasicIo &io = BasicIo::getInstance();

  __DEBUG(level, a);
  __DEBUG(level, b);

  size_t pivot = (a.size >> 1);
  size_t majorPivot = a.size;
  size_t bufferPivot = (a.size << 1);

  if (pivot == 0) {
    _wordBuffer.major = a.data[0];
    _wordBuffer.major *= b.data[0];
    c.clear();
    c.data[0] = _wordBuffer.minor.low;
    c.data[1] = _wordBuffer.minor.high;
    __DEBUG(level, c);
    return;
  }

  SourceBuffer la = a.splice(0, pivot);
  SourceBuffer ha = a.splice(pivot, pivot);
  SourceBuffer lb = b.splice(0, pivot);
  SourceBuffer hb = b.splice(pivot, pivot);

  SourceBuffer lc = c.splice(0, majorPivot);
  SourceBuffer hc = c.splice(majorPivot, majorPivot);

  SourceBuffer lBuffer = d.splice(0, pivot);
  SourceBuffer hBuffer = d.splice(pivot, pivot);
  SourceBuffer xBuffer = d.splice(majorPivot, majorPivot);
  SourceBuffer rBuffer = d.splice(bufferPivot, bufferPivot);

  __DEBUG(level, la);
  __DEBUG(level, ha);
  __DEBUG(level, lb);
  __DEBUG(level, hb);

  karIt(la, lb, lc, rBuffer, level + 1);
  karIt(ha, hb, hc, rBuffer, level + 1);

  __DEBUG(level, lc);
  __DEBUG(level, hc);

  sub(ha, la, lBuffer);
  bool lSign = !overflow();
  sub(lb, hb, hBuffer);
  bool hSign = !overflow();

  __DEBUG(level, lBuffer);
  __DEBUG(level, hBuffer);

  if (lSign) {
    invert(lBuffer);
  }
  if (hSign) {
    invert(hBuffer);
  }

  __DEBUG(level, lBuffer);
  __DEBUG(level, hBuffer);

  karIt(lBuffer, hBuffer, xBuffer, rBuffer, level + 1);

  __DEBUG(level, xBuffer);

  if (lSign != hSign) {
    invert(xBuffer);
  }

  __DEBUG(level, xBuffer);

  SourceBuffer c34 = c.splice(pivot, pivot * 3);

  __DEBUG(level, c34);
  __DEBUG(level, lc);
  unsigned_add(c34, lc);
  __DEBUG(level, c34);
  __DEBUG(level, hc);
  unsigned_add(c34, hc);
  __DEBUG(level, c34);
  __DEBUG(level, xBuffer);
  unsigned_add(c34, xBuffer);
  __DEBUG(level, c34);
  __DEBUG(level, c);
}
