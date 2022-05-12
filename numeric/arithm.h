#ifndef ARITHM
#define ARITHM
#include "buffer.h"

class Arithm {
  using BaseBuffer = Buffer<BaseType>;
  using SourceBuffer = const Buffer<BaseType> &;

  union DoubleBuffer {
    BufferType major;
    struct {
      BaseType low;
      BaseType high;
    } minor;
  };

  DoubleBuffer _wordBuffer = {0};
  BaseBuffer _aBuffer;
  BaseBuffer _bBuffer;

  static Arithm _instance;
  static constexpr size_t wordSize = sizeof(BaseType) * 8;

  static size_t min(size_t a, size_t b);

  Arithm();
  bool overflow();
  void karIt(SourceBuffer a, SourceBuffer b, SourceBuffer c, SourceBuffer d,
             int level = 0);

public:
  bool equal(SourceBuffer a, SourceBuffer b);
  bool less(SourceBuffer a, SourceBuffer b);
  bool greater(SourceBuffer a, SourceBuffer b);

  bool isSigned(SourceBuffer b);

  size_t leftOffset(SourceBuffer a);
  size_t rightOffset(SourceBuffer a);

  // Arithmetic functions, size of operands is in descending order from left
  void subLeft(SourceBuffer a, SourceBuffer b);
  void subRight(SourceBuffer a, SourceBuffer b);

  void addLeft(SourceBuffer a, SourceBuffer b);
  void addRight(SourceBuffer a, SourceBuffer b);

  void unsignedAddLeft(SourceBuffer a, SourceBuffer b);
  void invert(SourceBuffer b);

  void leftShift(SourceBuffer a, SourceBuffer b, size_t offset);
  void rightShift(SourceBuffer a, SourceBuffer b, size_t offset);

  void mul(SourceBuffer b, SourceBuffer a);
  void div(SourceBuffer a, SourceBuffer b, SourceBuffer c);
  void mod(SourceBuffer a, SourceBuffer b, SourceBuffer c);

  void kar(SourceBuffer a, SourceBuffer b, SourceBuffer c);

  void newRaph(SourceBuffer a, SourceBuffer b, SourceBuffer c);
  void modExp(SourceBuffer a,SourceBuffer b,SourceBuffer c);
  void gcd(SourceBuffer a, SourceBuffer b);

  static Arithm &getInstance(); // Will be moved to arithm provider
};

#endif /*ARITHM*/
