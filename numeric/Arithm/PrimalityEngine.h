#ifndef PRIMALITY_ENGINE
#define PRIMALITY_ENGINE

#include "../Buffer/BufferView.h"

namespace KCrypt {

class Buffer;
class BufferView;
class Numeric;

class ExpEngine;
class CompEngine;
class AddEngine;
class DivEngine;
class MulEngine;
class ArithmFacade;
class IoEngine;

class PrimalityEngine {
  CompEngine &_cmp;
  AddEngine &_add;
  MulEngine &_mul;
  DivEngine &_div;
  ExpEngine &_exp;
  IoEngine &_io;

  size_t _binPoint;
  size_t _powerOf2;

  Buffer &_modulusBuffer;
  Buffer &_modulusInverseBuffer;
  Buffer &_mulResultBuffer;
  Buffer &_resultBuffer;

  BufferView _modulusInverse;
  BufferView _modulus;
  BufferView _mulResult;
  BufferView _result;

public:
  PrimalityEngine(ArithmFacade &arithm);

  bool test(Numeric &witness);
  void setSuspect(Numeric &buffer);
};
} // namespace KCrypt

#endif /*PRIMALITY_ENGINE*/
