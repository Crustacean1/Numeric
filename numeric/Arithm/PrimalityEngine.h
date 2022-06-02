#ifndef PRIMALITY_ENGINE
#define PRIMALITY_ENGINE

namespace KCrypt {

class Buffer;
class BufferView;

class ExpEngine;
class CompEngine;

class PrimalityEngine {
public:
  PrimalityEngine();
  bool millerRabinTest(const BufferView & suspect, const BufferView & witness);
};
} // namespace KCrypt

#endif /*PRIMALITY_ENGINE*/
