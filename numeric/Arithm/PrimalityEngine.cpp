#include "PrimalityEngine.h"
#include "../BasicIo.h"
#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"
#include "../Numeric.h"
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
      _modBuffer(arithm.getBuffer(5)), _modInvBuffer(arithm.getBuffer(6)),
      _stumpBuffer(arithm.getBuffer(7)), _resultBuffer(arithm.getBuffer(8)),
      _modView(_modBuffer), _modInvView(_modInvBuffer),
      _resultView(_resultBuffer), _stumpView(_stumpBuffer) {}

bool PrimalityEngine::test(const BufferView &witness) {
  _exp.fastModExp(witness, _stumpView, _modView, _modInvView, _binPoint,
                  _resultView);
  std::cout << "Mod exponentat_ion: " << _io.toDecimal(witness) << "\t"
            << _io.toDecimal(_stumpView) << "\t" << _io.toDecimal(_modView)
            << "\t" << _io.toDecimal(_resultView) << std::endl;
  if (_cmp.equal(_resultView, BufferView::BaseInt(1))) {
    return true;
  }
  _add.sub(_modView, 1);
  for (size_t i = 0; i < _powerOf2; ++i) {
    std::cout << "Shift mod: " << _io.toDecimal(_resultView) << std::endl;
    if (_cmp.equal(_resultView, _modView)) {
      _add.add(witness, 1);
      return true;
    }
    _add.leftShift(_resultView, _resultView, 1);
  }
  _add.add(_modView, 1);
  return false;
}

void PrimalityEngine::setSuspect(BufferView &buffer) {
  _modBuffer.reserve(buffer.size);
  _stumpBuffer.reserve(buffer.size);
  _modInvBuffer.reserve(buffer.size);
  _resultBuffer.reserve(buffer.size);

  _modView = _modBuffer.splice(buffer.size);
  _modInvView = _modInvBuffer.splice(buffer.size);
  _stumpView = _stumpBuffer.splice(buffer.size);
  _resultView = _resultBuffer.splice(buffer.size);

  _modView.copy(buffer);
  _stumpView.copy(buffer);

  _add.sub(_stumpView, 1);
  _powerOf2 = _cmp.rightOffset(_stumpView);
  _add.rightShift(_modView, _stumpView, _powerOf2);

  _binPoint = _div.newtonInverse(_modView, _modInvView);
}
