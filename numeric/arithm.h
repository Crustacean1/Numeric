#ifndef ARITHM
#define ARITHM
#include "buffer.h"

class Arithm {
  using BaseBuffer = Buffer<BaseType>;
  using SourceBuffer = const Buffer<BaseType>&;
  using OutputBuffer = Buffer<BaseType>&;

  union DoubleBuffer {
    BufferType major;
    BaseType minor[2];
  };

  DoubleBuffer _wordBuffer = {0};
  BaseBuffer _aBuffer;
  BaseBuffer _bBuffer;

  static Arithm _instance;
  static constexpr size_t wordSize = sizeof(BaseType) * 8;

  static size_t min(size_t a, size_t b);

  Arithm();

public:
  bool equal(const Buffer<BaseType> &a, const Buffer<BaseType> &b);
  bool less(const Buffer<BaseType> &a, const Buffer<BaseType> &b);
  bool greater(const Buffer<BaseType> &a, const Buffer<BaseType> &b);

  bool isSigned(const Buffer<BaseType> &b);

  size_t leftOffset(SourceBuffer a);
  size_t rightOffset(SourceBuffer a);

  // Operators
  void sub(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
           Buffer<BaseType> &s);

  void add(const Buffer<BaseType> &a, const Buffer<BaseType> &b,
           Buffer<BaseType> &s);
  void invert(Buffer<BaseType> &b);

  void leftShift(const Buffer<BaseType> &a, Buffer<BaseType> &b, size_t offset);
  void rightShift(const Buffer<BaseType> &a, Buffer<BaseType> &b,
                  size_t offset);

  void mul(const Buffer<BaseType> &a,const Buffer<BaseType> &b,Buffer<BaseType> &c);
  void div(const Buffer<BaseType> &a,const Buffer<BaseType> &b,Buffer<BaseType> &c);
  void mod(SourceBuffer a,SourceBuffer b,OutputBuffer c);

  static Arithm &getInstance(); // Will be moved to arithm provider
};

#endif /*ARITHM*/
