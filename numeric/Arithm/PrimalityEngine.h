#ifndef PRIMALITY_ENGINE
#define PRIMALITY_ENGINE

#include "../Buffer/BufferView.h"

namespace KCrypt {

class Buffer;
class BufferView;

class ExpEngine;
class CompEngine;
class AddEngine;
class DivEngine;

class PrimalityEngine {
  CompEngine &_cmp;
  AddEngine &_add;
  DivEngine &_div;
  ExpEngine &_exp;

  size_t _binPoint;
  size_t _powerOf2;

  Buffer &_modBuffer;
  Buffer &_modInvBuffer;
  Buffer &_stumpBuffer;
  Buffer &_resultBuffer;

  BufferView _modInvView;
  BufferView _modView;
  BufferView _stumpView;
  BufferView &_resultView;

public:
  PrimalityEngine(CompEngine &cmp, AddEngine &add, DivEngine &div,
                  ExpEngine &exp, Buffer &modBuffer, Buffer &modInvBuffer,
                  Buffer &stumpBuffer, Buffer &resultBuffer);

  bool test(const BufferView &witness);
  void setSuspect(BufferView &buffer);
};
} // namespace KCrypt

#endif /*PRIMALITY_ENGINE*/
