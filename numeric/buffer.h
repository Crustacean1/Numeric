#ifndef BUFFER
#define BUFFER

#include <cstdint>
#include <cstddef>

using BaseType = uint32_t;

template<typename T>
struct Buffer{
  T * data;
  size_t size;
};

#endif /*BUFFER*/