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
