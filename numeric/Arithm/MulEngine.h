#ifndef MUL_ENGINE
#define MUL_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt {

class CompEngine;
class AddEngine;
class BufferInstance;

class MulEngine {
  DoubleBuffer _buffer;

  CompEngine &_comp;
  AddEngine &_adder;

  IntBuffer &karBuffer;

  void karIt(const IntBufferView &factorA, const IntBufferView &factorB,
             const IntBufferView &product, const IntBufferView &buffer,
             size_t level = 0);

public:
  MulEngine(CompEngine &comp, AddEngine &add, IntBuffer &kBuffer);


  void mul(const IntBufferView &factor, const IntBufferView &product);

  void kar(const IntBufferView &factorA, const IntBufferView &factorB,
           const IntBufferView &product);
};

} // namespace KCrypt
#endif /*MUL_ENGINE*/
