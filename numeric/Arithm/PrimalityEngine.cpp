#include "PrimalityEngine.h"
#include "../Buffer/Buffer.h"
#include "../Buffer/BufferView.h"

#include "AddEngine.h"
#include "CompEngine.h"
#include "DivEngine.h"
#include "ExpEngine.h"

using namespace KCrypt;

PrimalityEngine::PrimalityEngine(CompEngine &cmp, AddEngine &add,
                                 DivEngine &div, ExpEngine &exp,
                                 Buffer &modBuffer, Buffer &modInvBuffer,
                                 Buffer &stumpBuffer, Buffer &resultBuffer)
    : _cmp(cmp), _add(add), _div(div), _exp(exp), _modBuffer(modBuffer),
      _modInvBuffer(modInvBuffer), _stumpBuffer(stumpBuffer),
      _resultBuffer(resultBuffer), _modView(_modBuffer),
      _modInvView(_modInvBuffer), _resultView(_resultBuffer),
      _stumpView(_stumpBuffer) {}

bool PrimalityEngine::test(const BufferView &witness) {
  _exp.fastModExp(witness, _stumpView, _modView, _modInvView, _binPoint,
                  _resultView);
  if (_cmp.equal(_resultView, BufferView::BaseInt(1))) {
    return true;
  }
  for(size_t i = 0;i<_powerOf2;++i){
    _add.leftShift(_resultView,_resultView,1);

  }
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
