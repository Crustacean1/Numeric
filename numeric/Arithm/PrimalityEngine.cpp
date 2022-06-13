#include "PrimalityEngine.h"
#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"
#include "../Numeric.h"
#include "IoEngine.h"
#include <iostream>

#include "../ArithmFacade.h"
#include "AddEngine.h"
#include "CompEngine.h"
#include "DivEngine.h"
#include "ExpEngine.h"
#include "MulEngine.h"

using namespace KCrypt;

PrimalityEngine::PrimalityEngine(ArithmFacade &arithm)
    : _cmp(arithm.getCmp()), _add(arithm.getAdd()), _mul(arithm.getMul()),
      _div(arithm.getDiv()), _exp(arithm.getExp()), _io(arithm.getIo()),
      _modulusBuffer(1), _modulusInverseBuffer(1), _mulResultBuffer(1),
      _resultBuffer(1), _witnessBuffer(1), _modulus(_modulusBuffer),
      _modulusInverse(_modulusInverseBuffer), _result(_resultBuffer),
      _witness(_witnessBuffer),
      _mulResult(_mulResultBuffer), _primes{2,  3,  5,  7,  11, 13, 17, 19,
                                            23, 29, 31, 37, 41, 43, 47, 53,
                                            59, 61, 71, 73, 79, 83, 89, 97} {}

bool PrimalityEngine::fastPrimeTest(const BufferView &candidate) {
  for (const auto smallPrime : _primes) {
    if (_div.modulo(candidate, smallPrime) == 0) {
      return false;
    }
  }
  return true;
}

bool PrimalityEngine::millerRabinTest(const BufferView &witness) {

  _exp.fastModExp(witness, _mulResult.splice(_result.size), _modulus,
                  _modulusInverse, _binPoint, _result);

  // std::cout<<"RESULT: "<<_io.toDecimal(_result)<<std::endl;

  if (_cmp.equal(_result, BufferView::BaseInt(1))) {
    return true;
  }

  for (size_t i = 0; i < _powerOf2; ++i) {
    _add.add(_result, 1);
    if (_cmp.equal(_result, _modulus)) {
      return true;
    }
    _add.sub(_result, 1);
    _mul.kar(_result, _result, _mulResult);
    _div.fastModulo(_mulResult, _modulus, _modulusInverse, _result, _binPoint);
  }
  return false;
}

void PrimalityEngine::setSuspect(const BufferView &buffer) {
  size_t baseSize = buffer.size;

  _modulusBuffer.reserve(baseSize);
  _mulResultBuffer.reserve(baseSize * 2);
  _modulusInverseBuffer.reserve(baseSize * 2);
  _resultBuffer.reserve(baseSize);

  _modulus = _modulusBuffer.splice(baseSize);
  _modulusInverse = _modulusInverseBuffer.splice(baseSize * 2);
  _mulResult = _mulResultBuffer.splice(baseSize * 2);
  _result = _resultBuffer.splice(baseSize);

  _modulus.copy(buffer);
  _mulResult.copy(buffer);

  _add.sub(_mulResult, 1);
  _powerOf2 = _cmp.rightOffset(_mulResult);
  _add.rightShift(_modulus, _mulResult, _powerOf2);

  _binPoint = _div.newtonInverse(_modulus, _modulusInverse);
}
