#include "Numeric.h"

namespace KCrypt {

Numeric::Numeric()
    : _buffInst(BufferInstance::getInstance()), _compModule(),
      _addModule(_compModule), _mulModule(_compModule, _addModule, _buffInst[0],
                                          _buffInst[1], _buffInst[2]),
      _buffer(Buffer<BaseInt>::createBuffer(1)) {}

Numeric::Numeric(size_t size, BaseInt defaultValue)
    : _buffInst(BufferInstance::getInstance()), _compModule(),
      _addModule(_compModule), _mulModule(_compModule, _addModule, _buffInst[0],
                                          _buffInst[1], _buffInst[2]),
      _buffer(Buffer<BaseInt>::createBuffer(size)) {
  _buffer.clear();
  _buffer.data[0] = defaultValue;
}

Numeric::Numeric(Numeric &&num)
    : _buffInst(BufferInstance::getInstance()), _compModule(),
      _addModule(_compModule), _mulModule(_compModule, _addModule, _buffInst[0],
                                          _buffInst[1], _buffInst[2]),
      _buffer(num._buffer) {
  num._buffer.disownBuffer();
}

Numeric::Numeric(Buffer<BaseInt> buffer)
    : _buffInst(BufferInstance::getInstance()), _compModule(),
      _addModule(_compModule), _mulModule(_compModule, _addModule, _buffInst[0],
                                          _buffInst[1], _buffInst[2]),
      _buffer(buffer) {}

Numeric::Numeric(const std::string &str)
    : _buffInst(BufferInstance::getInstance()), _compModule(),
      _addModule(_compModule), _mulModule(_compModule, _addModule, _buffInst[0],
                                          _buffInst[1], _buffInst[2]),
      _buffer(_ioModule.toComplement(str, _addModule)) {}

Numeric::Numeric(const Numeric &num)
    : _buffInst(BufferInstance::getInstance()), _compModule(),
      _addModule(_compModule), _mulModule(_compModule, _addModule, _buffInst[0],
                                          _buffInst[1], _buffInst[2]),
      _buffer(IntBuffer::createBuffer(num.size())) {
  _buffer.copy(num._buffer);
}

Numeric &Numeric::operator=(const Numeric &num) {
  _buffer.copy(num._buffer);
  return *this;
}

Numeric &Numeric::operator=(Numeric &&num) {
  _buffer = num._buffer;
  num._buffer.disownBuffer();
  return *this;
}

bool Numeric::operator==(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _compModule.equal(this->_buffer, num._buffer);
  }
  return _compModule.equal(num._buffer, this->_buffer);
}

bool Numeric::operator<(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _compModule.lesser(_buffer, num._buffer);
  }
  return !_compModule.lesser(num._buffer, _buffer);
}

bool Numeric::operator>(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _compModule.greater(_buffer, num._buffer);
  }
  return !_compModule.greater(num._buffer, _buffer);
}

size_t Numeric::size() const { return _buffer.size; }

Numeric::~Numeric() { _buffer.releaseBuffer(); }

std::string Numeric::toDec() {
  return _ioModule.toDecimal(_buffer, _compModule);
}
std::string Numeric::toBin() { return _ioModule.toBinary(_buffer); }

Numeric Numeric::operator+(const Numeric &num) const {
  Numeric sum(_buffer.size);
  sum._buffer.copy(_buffer);
  if (sum._buffer.size < num._buffer.size) {
    sum._addModule.addToRight(num._buffer, sum._buffer);
    return sum;
  }
  sum._addModule.addToLeft(sum._buffer, num._buffer);
  return sum;
}

Numeric Numeric::operator-(const Numeric &num) const {
  Numeric sum(_buffer.size);
  sum._buffer.copy(_buffer);
  if (sum._buffer.size < num._buffer.size) {
    sum._addModule.subFromRight(num._buffer, sum._buffer);
    return sum;
  }
  sum._addModule.subFromLeft(sum._buffer, num._buffer);
  return sum;
}

Numeric &Numeric::operator+=(const Numeric &num) {
  if (_buffer.size < num._buffer.size) {
    _addModule.addToRight(num._buffer, _buffer);
    return *this;
  }
  _addModule.addToLeft(_buffer, num._buffer);
  return *this;
}

Numeric &Numeric::operator-=(const Numeric &num) {
  if (_buffer.size < num._buffer.size) {
    _addModule.subFromRight(num._buffer, _buffer);
    return *this;
  }
  _addModule.subFromLeft(_buffer, num._buffer);
  return *this;
}

Numeric &Numeric::operator<<=(const Numeric &num) {
  _mulModule.leftShift(_buffer, _buffer, num._buffer.data[0]);
  return *this;
}

Numeric &Numeric::operator>>=(const Numeric &num) {
  _mulModule.rightShift(_buffer, _buffer, num._buffer.data[0]);
  return *this;
}

Numeric Numeric::operator*(const Numeric &num) const {
  Numeric result(size());
  _buffInst[3].reserve(size() * 2);

  if (size() > num.size()) {
    result._mulModule.kar(_buffer, num._buffer, _buffInst[3]);
    result._buffer.copy(_buffInst[3]);
    return result;
  }
  result._mulModule.kar(num._buffer, _buffer, _buffInst[3]);
  result._buffer.copy(_buffInst[3]);
  return result;
}

Numeric &Numeric::orderedMul(const IntBuffer &a, const IntBuffer &b,
                             IntBuffer &c) {
  c.reserve(a.size * 2);
  _mulModule.kar(a, b, c);
  _buffer.copy(c);
  return *this;
}

Numeric &Numeric::operator*=(const Numeric &num) {
  if (size() > num.size()) {
    return orderedMul(_buffer,num._buffer,_buffInst[3]);
  }
  return orderedMul(num._buffer,_buffer,_buffInst[3]);
}

Numeric Numeric::operator/(const Numeric &num) const {
  Numeric result(size());
  result._mulModule.div(_buffer, num._buffer, result._buffer);
  return result;
}

Numeric &Numeric::operator/=(const Numeric &num) {
  _mulModule.div(_buffer, num._buffer, _buffer);

  //_buffInst[3].reserve(_buffer.size);
  //size_t precision = _mulModule.newtonInverse(num._buffer, _buffInst[3].splice(0,size()));
  //_mulModule.newtonDiv(_buffer, _buffInst[3], _buffer, precision);
  return *this;
}

bool Numeric::isSigned() { return _compModule.isSigned(_buffer); }

} // namespace KCrypt

std::ostream &operator<<(std::ostream &stream, const KCrypt::Numeric &num) {
  stream << num._ioModule.toDecimal(num._buffer, num._compModule);
  return stream;
}

std::istream &operator>>(std::istream &stream, KCrypt::Numeric &num) {
  std::string buffer;
  stream >> buffer;
  num._buffer = num._ioModule.toComplement(buffer, num._addModule);
  return stream;
}
