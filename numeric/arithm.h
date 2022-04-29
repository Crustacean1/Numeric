#ifndef ARITHM
#define ARITHM
#include "buffer.h"

class Arithm {
  using BaseBuffer = Buffer<BaseType>;
  using SourceBuffer = const Buffer<BaseType>&;

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
void karIt(SourceBuffer a,SourceBuffer b, SourceBuffer c);

public:
  bool equal(SourceBuffer a, SourceBuffer b);
  bool less(SourceBuffer a, SourceBuffer b);
  bool greater(SourceBuffer a, SourceBuffer b);

  bool isSigned(SourceBuffer b);

  size_t leftOffset(SourceBuffer a);
  size_t rightOffset(SourceBuffer a);

  // Operators
  void sub(SourceBuffer a, SourceBuffer b,
           SourceBuffer s);

  void add(SourceBuffer a, SourceBuffer b,
           SourceBuffer s);
  void invert(SourceBuffer b);

  void leftShift(SourceBuffer a, SourceBuffer b, size_t offset);
  void rightShift(SourceBuffer a, SourceBuffer b,
                  size_t offset);

  void mul(SourceBuffer b, SourceBuffer a);
  void div(SourceBuffer a,SourceBuffer b,SourceBuffer c);
  void mod(SourceBuffer a,SourceBuffer b,SourceBuffer c);

  void kar(SourceBuffer a,SourceBuffer b,  SourceBuffer c);

  void newRaph(SourceBuffer a,SourceBuffer b);

  static Arithm &getInstance(); // Will be moved to arithm provider
};

#endif /*ARITHM*/
