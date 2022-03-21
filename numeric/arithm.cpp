#include "arithm.h"

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
