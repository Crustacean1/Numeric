#include "arithm.h"

Arithm Arithm::_instance;

Arithm &Arithm::getInstance() { return _instance; }

size_t Arithm::min(size_t a, size_t b) {
  bool min = a < b;
  return a * min + (!min) * b;
}

bool Arithm::equal(const Buffer<BaseType> &a, const Buffer<BaseType> &b) {
  for (size_t i = 0; i < min(a.size, b.size); ++i) {
    if (a.data[i] != b.data[i]) {
      return false;
    }
  }
  for (size_t i = a.size; i < b.size; ++i) {
    if (b.data[i] != 0) {
      return false;
    }
  }
  for (size_t i = b.size; i < a.size; ++i) {
    if (a.data[i] != 0) {
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
  _buffer = 0;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;

  for (i = 0; i < minSize; ++i) {
    _buffer += a.data[i];
    _buffer += b.data[i];
    s.data[i] = _buffer;
    _buffer >>= wordSize;
  }
  for (; i < a.size; ++i) {
    _buffer += a.data[i];
    s.data[i] = _buffer;
    _buffer >>= wordSize;
  }
  for (; i < b.size; ++i) {
    _buffer += b.data[i];
    s.data[i] = _buffer;
    _buffer >>= wordSize;
  }
  if (i < s.size) {
    s.data[++i] = _buffer;
  }
}

void Arithm::sub(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
                 Buffer<BaseType> &s) {
  constexpr size_t wordSize = sizeof(BaseType) * 8;
  _buffer = 1;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;

  for (i = 0; i < minSize; ++i) {
    _buffer += a.data[i];
    _buffer += (~b.data[i]);
    s.data[i] = _buffer;
    _buffer >>= wordSize;
  }
  for (; i < a.size; ++i) {
    _buffer += a.data[i];
    s.data[i] = _buffer;
    _buffer >>= wordSize;
  }
  for (; i < b.size; ++i) {
    _buffer += (~b.data[i]);
    s.data[i] = _buffer;
    _buffer >>= wordSize;
  }
  if (i < s.size) {
    s.data[++i] = _buffer;
  }
}
void Arithm::invert(Buffer<BaseType> &integer) {
  _buffer = 1;
  constexpr size_t wordSize = sizeof(BaseType) * 8;
  for (size_t i = 0; i < integer.size; ++i) {
    _buffer += integer.data[i];
    integer.data[i] = _buffer;
    _buffer >>= wordSize;
  }
}

bool Arithm::isSigned(const Buffer<BaseType> &buffer) {
  constexpr size_t signPos = sizeof(BaseType) * 8 - 1;
  return (buffer.data[buffer.size - 1] >> signPos) & 1;
}

void Arithm::leftShift(const Buffer<BaseType> &a, Buffer<BaseType> &b,
                       size_t offset) {
  size_t majorOffset = offset / BaseWordSize;
  size_t minorOffset = offset - majorOffset * BaseWordSize;
  size_t compMinorOffset = BaseWordSize - minorOffset;
  size_t leftPos = (a.size - 1 - majorOffset) * (a.size > majorOffset + 1);
  size_t shiftBuffer;
  size_t i;

  _buffer = a.data[leftPos--];
  _buffer <<= minorOffset;

  /*for(i = a.size;i>=majorOffset;--i,--leftPos){
    shiftBuffer = a.data[leftPos];
    shiftBuffer >>= compMinorOffset;
    _buffer += shiftBuffer;
    b.data[i-1] = _buffer;
    _buffer <<= BaseWordSize;
  }
  for(;i>0;--i){
    b.data[i-1] = 0;
  }

  memset(b.data,0,majorOffset*sizeof(BaseType));*/
}
void Arithm::rightShift(const Buffer<BaseType> &a, Buffer<BaseType> &b,
                        size_t offset) {
  /*size_t minorOffset = offset%BaseWordSize;
  size_t compMinorOffset = BaseWordSize - minorOffset;
  size_t majorOffset = offset/BaseWordSize;
  size_t shiftBuffer;

  size_t i;

  _buffer = a.data[majorOffset];
  _buffer<<=minorOffset;

  for(size_t i = 0;i<=a.size - majorOffset ;++i){
    shiftBuffer = a.data[i + majorOffset];
    shiftBuffer >>= compMinorOffset;
    _buffer += shiftBuffer;
    b.data[i] = _buffer;
    _buffer <<= BaseWordSize;*
  }
  for(;i<a.size;++i){
    b.data[i] = 0;
  }
  memset(b.data + a.size - majorOffset,
      majorOffset,
      majorOffset*sizeof(BaseType));*/
}
