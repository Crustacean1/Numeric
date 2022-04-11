#ifndef ARITHM
#define ARITHM
#include "buffer.h"

class Arithm {
  union DoubleBuffer {
    BufferType majorBuffer;
    BaseType minorBuffer[2];
  };

  const Buffer<BaseType> *_smallTerm;
  const Buffer<BaseType> *_bigTerm;
  DoubleBuffer _buffer = {0};

  static Arithm _instance;

  public : static size_t
           min(size_t a, size_t b);
  bool equal(const Buffer<BaseType> &a, const Buffer<BaseType> &b);
  bool less(const Buffer<BaseType> &a, const Buffer<BaseType> &b);
  bool greater(const Buffer<BaseType> &a, const Buffer<BaseType> &b);

  void sub(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
           Buffer<BaseType> &s);

  void add(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
           Buffer<BaseType> &s);
  void invert(Buffer<BaseType> &b);

  void leftShift(const Buffer<BaseType> &a, Buffer<BaseType> &b, size_t offset);
  void rightShift(const Buffer<BaseType> &a, Buffer<BaseType> &b,
                  size_t offset);

  bool isSigned(const Buffer<BaseType> &b);

  static Arithm &getInstance();
};

#endif /*ARITHM*/
