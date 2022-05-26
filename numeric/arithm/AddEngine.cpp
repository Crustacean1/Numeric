#include "AddEngine.h"
#include "../Utils.h"
#include <iostream>

namespace KCrypt {

AddEngine::AddEngine(Comparator &comp) : _comparator(comp) {}

void AddEngine::addToLeft(const IntBuffer a, const IntBuffer b) {
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

void AddEngine::addToRight(const IntBuffer a, const IntBuffer b) {
  _buffer.major = 0;
  size_t i = 0;
  for (i = 0; i < b.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += b.data[i];
    b.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

void AddEngine::addUnsignedToLeft(const IntBuffer a, const IntBuffer b) {
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

void AddEngine::subFromLeft(const IntBuffer a, const IntBuffer b) {
  _buffer.major = 1;
  size_t i = 0;

  for (i = 0; i < b.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += (~b.data[i]);
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
  BaseInt padding = (!_comparator.isSigned(b)) * BaseInt(~0);
  for (; i < a.size; ++i) {
    _buffer.major += a.data[i];
    _buffer.major += padding;
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

void AddEngine::subFromRight(const IntBuffer a, const IntBuffer b) {
  _buffer.major = 1;
  size_t i = 0;

  for (i = 0; i < b.size; ++i) {
    _buffer.major += (~a.data[i]);
    _buffer.major += b.data[i];
    b.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

void AddEngine::invert(const IntBuffer a) {
  _buffer.major = 1;
  for (size_t i = 0; i < a.size; ++i) {
    _buffer.major += ~a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

void AddEngine::add(IntBuffer a, BaseInt b) {
  _buffer.major = b;

  for (size_t i = 0; _buffer.major != 0 && i < a.size; ++i) {
    _buffer.major += a.data[i];
    a.data[i] = _buffer.minor.low;
    _buffer.major >>= wordSize;
  }
}

bool AddEngine::overflow() { return _buffer.major & 1; }
} // namespace KCrypt
