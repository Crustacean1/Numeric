
#ifndef ARITHM_INJECTOR
#define ARITHM_INJECTOR

#include "Arithm/AddEngine.h"
#include "Arithm/CompEngine.h"
#include "Arithm/DivEngine.h"
#include "Arithm/ExpEngine.h"
#include "Arithm/GcdEngine.h"
#include "Arithm/IoEngine.h"
#include "Arithm/MulEngine.h"
#include "Arithm/PrimalityEngine.h"
#include "Arithm/RsaEngine.h"

#include <thread>
#include <mutex>
#include <unordered_map>

namespace KCrypt {

class ArithmInjector {

  static std::unordered_map<std::thread::id, ArithmInjector *> _instances;
  static std::mutex _injectorMutex;

  PrimalityEngine _pri;
  CompEngine _cmp;
  IoEngine _io;
  AddEngine _add;
  MulEngine _mul;
  DivEngine _div;
  ExpEngine _exp;
  GcdEngine _gcd;
  RsaEngine _rsa;

  Buffer _tmpBuffer;
  static ArithmInjector &getInstance(std::thread::id id);

public:
  ArithmInjector();
  ArithmInjector(const ArithmInjector &) = delete;
  ArithmInjector &operator=(const ArithmInjector &) = delete;

  static ArithmInjector &getInstance();

  static void releaseInstances();

  AddEngine &getAdd();
  CompEngine &getCmp();
  MulEngine &getMul();
  DivEngine &getDiv();
  ExpEngine &getExp();
  GcdEngine &getGcd();
  IoEngine &getIo();
  Buffer &getTmp();
  RsaEngine &getRsa();
  PrimalityEngine &getPrim();
};
} // namespace KCrypt

#endif /*ARITHM_INJECTOR*/
