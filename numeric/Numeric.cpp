#include "Numeric.h"

namespace KCrypt {

Numeric::Numeric() : Numeric(IntBuffer(1)){}

Numeric::Numeric(const Buffer<BaseInt> &buffer)
    : _buffInst(BufferInstance::getInstance()), _cmp(), _add(_cmp),
      _io(_cmp, _add), _mul(_cmp, _add, _buffInst[0]),
      _div(_cmp, _add, _mul, _buffInst[1], _buffInst[2]),
      _exp(_cmp, _add, _mul, _div), _buffer(buffer) {}

Numeric::Numeric(Buffer<BaseInt> &&buffer)
    : _buffInst(BufferInstance::getInstance()), _cmp(), _add(_cmp),
      _io(_cmp, _add), _mul(_cmp, _add, _buffInst[0]),
      _div(_cmp, _add, _mul, _buffInst[1], _buffInst[2]),
      _exp(_cmp, _add, _mul, _div), _buffer(std::move(buffer)) {}

Numeric::Numeric(size_t size, BaseInt defaultValue) : Numeric(IntBuffer(size)) {
  _buffer.clear();
  _buffer.data[0] = defaultValue;
}

Numeric::Numeric(Numeric &&num) : Numeric(std::move(num._buffer)) {}

Numeric::Numeric(const Numeric &num) : Numeric(num._buffer) {}

Numeric::Numeric(const std::string &str) : Numeric() {
  _buffer.reserve(_io.decSizeInBinary(str.size()));
  _io.toComplement(str, _buffer);
}

Numeric &Numeric::operator=(const Numeric &num) {
  _buffer.copy(num._buffer);
  return *this;
}

Numeric &Numeric::operator=(Numeric &&num) {
  _buffer = std::move(num._buffer);
  return *this;
}

bool Numeric::operator==(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _cmp.equal(this->_buffer, num._buffer);
  }
  return _cmp.equal(num._buffer, this->_buffer);
}

bool Numeric::operator<(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _cmp.lesser(_buffer, num._buffer);
  }
  return !_cmp.lesser(num._buffer, _buffer);
}

bool Numeric::operator>(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _cmp.greater(_buffer, num._buffer);
  }
  return !_cmp.greater(num._buffer, _buffer);
}

size_t Numeric::size() const { return _buffer.size; }

Numeric::~Numeric() { _buffer.releaseBuffer(); }

std::string Numeric::toDec() { return _io.toDecimal(_buffer); }
std::string Numeric::toBin() { return _io.toBinary(_buffer); }

Numeric Numeric::operator+(const Numeric &num) const {
  Numeric sum(_buffer.size);
  sum._buffer.copy(_buffer);
  if (sum._buffer.size < num._buffer.size) {
    sum._add.addToRight(num._buffer, sum._buffer);
    return sum;
  }
  sum._add.addToLeft(sum._buffer, num._buffer);
  return sum;
}

Numeric Numeric::operator-(const Numeric &num) const {
  Numeric sum(_buffer.size);
  sum._buffer.copy(_buffer);
  if (sum._buffer.size < num._buffer.size) {
    sum._add.subFromRight(num._buffer, sum._buffer);
    return sum;
  }
  sum._add.subFromLeft(sum._buffer, num._buffer);
  return sum;
}

Numeric &Numeric::operator+=(const Numeric &num) {
  if (_buffer.size < num._buffer.size) {
    _add.addToRight(num._buffer, _buffer);
    return *this;
  }
  _add.addToLeft(_buffer, num._buffer);
  return *this;
}

Numeric &Numeric::operator-=(const Numeric &num) {
  if (_buffer.size < num._buffer.size) {
    _add.subFromRight(num._buffer, _buffer);
    return *this;
  }
  _add.subFromLeft(_buffer, num._buffer);
  return *this;
}

Numeric &Numeric::operator<<=(const Numeric &num) {
  _add.leftShift(_buffer, _buffer, num._buffer.data[0]);
  return *this;
}

Numeric &Numeric::operator>>=(const Numeric &num) {
  _add.rightShift(_buffer, _buffer, num._buffer.data[0]);
  return *this;
}

Numeric Numeric::operator*(const Numeric &num) const {
  Numeric result(size());

  if (size() > num.size()) {
    return result.orderedMul(result._buffer, num._buffer, _buffInst[3]);
  }
  return result.orderedMul(result._buffer, num._buffer, _buffInst[3]);
}

Numeric &Numeric::orderedMul(const IntBuffer &a, const IntBuffer &b,
                             IntBuffer &c) {
  c.reserve(a.size * 2);
  auto result = c.splice(0, a.size * 2);
  _mul.kar(a, b, c.splice(0, a.size * 2));
  _buffer.copy(result);
  return *this;
}

Numeric &Numeric::operator*=(const Numeric &num) {
  if (size() > num.size()) {
    return orderedMul(_buffer, num._buffer, _buffInst[3]);
  }
  return orderedMul(num._buffer, _buffer, _buffInst[3]);
}

Numeric Numeric::operator/(const Numeric &num) const {
  Numeric result(size());
  result._div.div(_buffer, num._buffer, result._buffer);
  return result;
}

Numeric &Numeric::operator/=(const Numeric &num) {
  _buffInst[3].reserve(_buffer.size);
  auto inverse = _buffInst[3].splice(0, size());
  size_t precision = _div.newtonInverse(num._buffer, inverse);
  _div.newtonDiv(_buffer, inverse, _buffer, precision);
  return *this;
}

bool Numeric::isSigned() { return _cmp.isSigned(_buffer); }

} // namespace KCrypt

std::ostream &operator<<(std::ostream &stream, const KCrypt::Numeric &num) {
  stream << num._io.toDecimal(num._buffer);
  return stream;
}

std::istream &operator>>(std::istream &stream, KCrypt::Numeric &num) {
  std::string buffer;
  stream >> buffer;
  num._buffer.reserve(num._io.decSizeInBinary(buffer.size()));
  num._io.toComplement(buffer, num._buffer);
  return stream;
}
