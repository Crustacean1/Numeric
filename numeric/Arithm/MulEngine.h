#ifndef MUL_ENGINE
#define MUL_ENGINE

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

namespace KCrypt {

class CompEngine;
class AddEngine;
class BufferInstance;
class ArithmFacade;

class MulEngine {
  DoubleBuffer _buffer;

  CompEngine &_comp;
  AddEngine &_adder;

  Buffer &karBuffer;

  void karIt(const BufferView &factorA, const BufferView &factorB,
             const BufferView &product, const BufferView &buffer);

public:
  MulEngine(ArithmFacade &arithm);

  void mul(const BufferView &factor, const BufferView &product);

  void kar(const BufferView &factorA, const BufferView &factorB,
           const BufferView &product);
};

} // namespace KCrypt
#endif /*MUL_ENGINE*/
