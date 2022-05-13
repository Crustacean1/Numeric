#include "numeric.h"

using namespace KCrypt;

Numeric::Numeric()
    : _ioModule(BasicIo::getInstance()), _arthModule(Arithm::getInstance()),
      _buffer(Buffer<BaseType>::createBuffer(1)) {}

Numeric::Numeric(size_t size, BaseType defaultValue)
    : _ioModule(BasicIo::getInstance()), _arthModule(Arithm::getInstance()),
      _buffer(Buffer<BaseType>::createBuffer(size)) {
  _buffer.clear();
  _buffer.data[0] = defaultValue;
}

Numeric::Numeric(Numeric &&num)
    : _ioModule(num._ioModule), _arthModule(Arithm::getInstance()),
      _buffer(std::move(num._buffer)) {}

Numeric::Numeric(Buffer<BaseType> &&buffer)
    : _ioModule(BasicIo::getInstance()), _arthModule(Arithm::getInstance()),
      _buffer(std::move(buffer)) {}

Numeric::Numeric(const std::string &str)
    : _ioModule(BasicIo::getInstance()), _arthModule(Arithm::getInstance()),
      _buffer(_ioModule.toBuffer(str, _arthModule)) {}

Numeric::Numeric(const Numeric &num)
    : _ioModule(BasicIo::getInstance()), _arthModule(Arithm::getInstance()),
      _buffer(num._buffer) {}

Numeric &Numeric::operator=(const Numeric &num) {
  _buffer = num._buffer;
  return *this;
}

Numeric &Numeric::operator=(Numeric &&num) {
  _buffer = std::move(num._buffer);
  return *this;
}

bool Numeric::operator==(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _arthModule.equal(this->_buffer, num._buffer);
  }
  return _arthModule.equal(num._buffer, this->_buffer);
}

bool Numeric::operator<(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _arthModule.less(_buffer, num._buffer);
  }
  return !_arthModule.less(num._buffer, _buffer);
}

bool Numeric::operator>(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _arthModule.greater(_buffer, num._buffer);
  }
  return !_arthModule.greater(num._buffer, _buffer);
}

size_t Numeric::size() const { return _buffer.size; }

Numeric::~Numeric() { _buffer.releaseBuffer(); }

std::string Numeric::toDec() { return _ioModule.getDec(_buffer, _arthModule); }
std::string Numeric::toBin() { return _ioModule.getBin(_buffer, _arthModule); }

Numeric Numeric::operator+(const Numeric &num) {
  Numeric sum(_buffer.size);
  sum._buffer.copy(_buffer);
  if (sum._buffer.size < num._buffer.size) {
    _arthModule.addRight(num._buffer, sum._buffer);
    return sum;
  }
  _arthModule.addLeft(sum._buffer, num._buffer);
  return sum;
}

Numeric Numeric::operator-(const Numeric &num) {
  Numeric sum(_buffer.size);
  sum._buffer.copy(_buffer);
  if (sum._buffer.size < num._buffer.size) {
    _arthModule.subRight(num._buffer, sum._buffer);
    return sum;
  }
  _arthModule.subLeft(sum._buffer, num._buffer);
  return sum;
}

Numeric &Numeric::operator+=(const Numeric &num) {
  if (_buffer.size < num._buffer.size) {
    _arthModule.addRight(num._buffer, _buffer);
    return *this;
  }
  _arthModule.addLeft(_buffer, num._buffer);
  return *this;
}

Numeric &Numeric::operator-=(const Numeric &num) {
  if (_buffer.size < num._buffer.size) {
    _arthModule.subRight(num._buffer, _buffer);
    return *this;
  }
  _arthModule.subLeft(_buffer, num._buffer);
  return *this;
}

Numeric &Numeric::operator<<=(const Numeric &num) {
  _arthModule.leftShift(_buffer, _buffer, num._buffer.data[0]);
  return *this;
}

Numeric &Numeric::operator>>=(const Numeric &num) {
  _arthModule.rightShift(_buffer, _buffer, num._buffer.data[0]);
  return *this;
}

Numeric Numeric::operator*(const Numeric &num) {
  Numeric result(size());
  _arthModule.kar(_buffer, num._buffer, _buffer);
  return result;
}

Numeric &Numeric::operator*=(const Numeric &num) {
  Buffer<BaseType> buffer = Buffer<BaseType>::createBuffer(_buffer.size);
  buffer.copy(_buffer);

  if(buffer.size > num._buffer.size){
    _arthModule.kar(buffer, num._buffer, _buffer);
    return *this;
  }
  _arthModule.kar(num._buffer, buffer, _buffer);
  return *this;
}

Numeric Numeric::operator/(const Numeric &num) {
  Numeric result(size());
  _arthModule.div(_buffer, num._buffer, result._buffer);
  return result;
}

Numeric &Numeric::operator/=(const Numeric &num) {
  //_arthModule.div(_buffer, num._buffer, _buffer);
  _arthModule.div(_buffer,num._buffer,_buffer);
  return *this;
}

bool Numeric::isSigned() { return _arthModule.isSigned(_buffer); }

std::ostream &operator<<(std::ostream &stream, const Numeric &num) {
  stream << num._ioModule.getDec(num._buffer, num._arthModule);
  return stream;
}

std::istream &operator>>(std::istream &stream, Numeric &num) {
  std::string buffer;
  stream >> buffer;
  num._buffer = num._ioModule.toBuffer(buffer, num._arthModule);
  return stream;
}
