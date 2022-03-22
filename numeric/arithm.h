#ifndef ARITHM
#define ARITHM
#include "buffer.h"

class Arithm {
  const Buffer<BaseType> * _smallTerm;
  const Buffer<BaseType> * _bigTerm;
  BufferType _buffer = 0;

  static Arithm _instance;

public:
  static size_t min(size_t a, size_t b);
  bool equal(const Buffer<BaseType> &a, const Buffer<BaseType> &b);
  void sub(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
                  Buffer<BaseType> &s);
  void add(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
                  Buffer<BaseType> &s);
  static Arithm& getInstance();

};


#endif /*ARITHM*/
