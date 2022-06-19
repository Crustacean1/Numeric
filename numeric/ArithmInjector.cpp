#include "ArithmInjector.h"
#include "Arithm/PrimalityEngine.h"
#include <iostream>

using namespace KCrypt;

std::unordered_map<std::thread::id, ArithmInjector *>
    ArithmInjector::_instances;

ArithmInjector &ArithmInjector::getInstance(std::thread::id id) {
  auto it = _instances.find(id);
  if (it == _instances.end()) {
    auto *newInstance = new ArithmInjector();
    _instances[id] = newInstance;
    return *newInstance;
  }
  return *_instances[id];
}

ArithmInjector &ArithmInjector::getInstance() {
  return getInstance(std::this_thread::get_id());
}

void ArithmInjector::releaseInstances() {
  for (auto &arth : _instances) {
    delete arth.second;
  }
  _instances.clear();
}

ArithmInjector::ArithmInjector()
    : _cmp(), _add(*this), _mul(*this), _io(*this), _div(*this), _exp(*this),
      _gcd(*this), _rsa(*this), _pri(*this) {}

AddEngine &ArithmInjector::getAdd() { return _add; }
CompEngine &ArithmInjector::getCmp() { return _cmp; }
MulEngine &ArithmInjector::getMul() { return _mul; }
DivEngine &ArithmInjector::getDiv() { return _div; }
ExpEngine &ArithmInjector::getExp() { return _exp; }
GcdEngine &ArithmInjector::getGcd() { return _gcd; }
IoEngine &ArithmInjector::getIo() { return _io; }
Buffer &ArithmInjector::getTmp() { return _tmpBuffer; }
RsaEngine &ArithmInjector::getRsa() { return _rsa; }
PrimalityEngine &ArithmInjector::getPrim() { return _pri; }
