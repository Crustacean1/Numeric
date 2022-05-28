#include "Numeric.h"

namespace KCrypt {

Numeric::Numeric() : Numeric(Buffer(1)) {}

Numeric::Numeric(const BufferView &buffer)
    : _buffInst(BufferInstance::getInstance()), _cmp(), _add(_cmp),
      _io(_cmp, _add), _mul(_cmp, _add, _buffInst[0]),
      _div(_cmp, _add, _mul, _buffInst[1], _buffInst[2]),
      _exp(_cmp, _add, _mul, _div, _buffInst[3], _buffInst[4]),
      _buffer(buffer.size) {
  _buffer.splice().copy(buffer);
}

Numeric::Numeric(Buffer &&bufferHandle)
    : _buffInst(BufferInstance::getInstance()), _cmp(), _add(_cmp),
      _io(_cmp, _add), _mul(_cmp, _add, _buffInst[0]),
      _div(_cmp, _add, _mul, _buffInst[1], _buffInst[2]),
      _exp(_cmp, _add, _mul, _div, _buffInst[3], _buffInst[4]),
      _buffer(std::move(bufferHandle)) {}

Numeric::Numeric(size_t size, BufferView::BaseInt defaultValue)
    : Numeric(Buffer(size)) {
  _buffer.splice().clear();
  _buffer.splice().data[0] = defaultValue;
}

Numeric::Numeric(const Numeric &num) : Numeric(num._buffer.splice()) {}

Numeric::Numeric(Numeric &&num) : Numeric(std::move(num._buffer)) {}

Numeric::Numeric(const std::string &str) : Numeric() {
  _buffer.reserve(_io.decSizeInBinary(str.size()));
  _io.toComplement(str, _buffer.splice());
}

Numeric &Numeric::operator=(const Numeric &num) {
  _buffer.splice().copy(num._buffer.splice());
  return *this;
}

bool Numeric::operator==(const Numeric &num) const {
  if (_buffer.splice().size > num._buffer.splice().size) {
    return _cmp.equal(_buffer.splice(), num._buffer.splice());
  }
  return _cmp.equal(num._buffer.splice(), _buffer.splice());
}

bool Numeric::operator<(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _cmp.lesser(_buffer.splice(), num._buffer.splice());
  }
  return !_cmp.lesser(num._buffer.splice(), _buffer.splice());
}

bool Numeric::operator>(const Numeric &num) const {
  if (_buffer.size > num._buffer.size) {
    return _cmp.greater(_buffer.splice(), num._buffer.splice());
  }
  return !_cmp.greater(num._buffer.splice(), _buffer.splice());
}

size_t Numeric::size() const { return _buffer.splice().size; }

Numeric::~Numeric() {}

std::string Numeric::toDec() { return _io.toDecimal(_buffer.splice()); }
std::string Numeric::toBin() { return _io.toBinary(_buffer.splice()); }

Numeric Numeric::operator+(const Numeric &num) const {
  Numeric sum(_buffer.splice().size);
  sum._buffer.splice().copy(_buffer.splice());

  if (sum._buffer.splice().size < num._buffer.splice().size) {
    sum._add.addToRight(num._buffer.splice(), sum._buffer.splice());
    return sum;
  }
  sum._add.addToLeft(sum._buffer.splice(), num._buffer.splice());
  return sum;
}

Numeric Numeric::operator-(const Numeric &num) const {
  Numeric sum(_buffer.splice().size);
  sum._buffer.splice().copy(_buffer.splice());

  if (sum._buffer.splice().size < num._buffer.splice().size) {
    sum._add.subFromRight(num._buffer.splice(), sum._buffer.splice());
    return sum;
  }

  sum._add.subFromLeft(sum._buffer.splice(), num._buffer.splice());
  return sum;
}

Numeric &Numeric::operator+=(const Numeric &num) {
  if (_buffer.splice().size < num._buffer.splice().size) {
    _add.addToRight(num._buffer.splice(), _buffer.splice());
    return *this;
  }
  _add.addToLeft(_buffer.splice(), num._buffer.splice());
  return *this;
}

Numeric &Numeric::operator-=(const Numeric &num) {
  if (_buffer.splice().size < num._buffer.splice().size) {
    _add.subFromRight(num._buffer.splice(), _buffer.splice());
    return *this;
  }
  _add.subFromLeft(_buffer.splice(), num._buffer.splice());
  return *this;
}

Numeric &Numeric::operator-=(const BufferView::BaseInt b) {
  _add.sub(_buffer, b);
  return *this;
}

Numeric &Numeric::operator<<=(const Numeric &num) {
  _add.leftShift(_buffer.splice(), _buffer.splice(),
                 num._buffer.splice().data[0]);
  return *this;
}

Numeric &Numeric::operator>>=(const Numeric &num) {
  _add.rightShift(_buffer.splice(), _buffer.splice(),
                  num._buffer.splice().data[0]);
  return *this;
}

Numeric Numeric::operator*(const Numeric &num) const {
  Numeric result(size());

  if (size() > num.size()) {
    return result.orderedMul(result._buffer.splice(), num._buffer.splice(),
                             _buffInst[3]);
  }
  return result.orderedMul(result._buffer.splice(), num._buffer.splice(),
                           _buffInst[3]);
}

Numeric &Numeric::orderedMul(const BufferView &a, const BufferView &b,
                             Buffer &c) {
  c.reserve(a.size * 2);
  auto result = c.splice(0, a.size * 2);
  _mul.kar(a, b, c.splice(0, a.size * 2));
  _buffer.splice().copy(result);
  return *this;
}

Numeric &Numeric::operator*=(const Numeric &num) {
  if (size() > num.size()) {
    return orderedMul(_buffer.splice(), num._buffer.splice(), _buffInst[3]);
  }
  return orderedMul(num._buffer.splice(), _buffer.splice(), _buffInst[3]);
}

Numeric Numeric::operator/(const Numeric &num) const {
  Numeric result(size());
  result._div.div(_buffer.splice(), num._buffer.splice(),
                  result._buffer.splice());
  return result;
}

Numeric &Numeric::operator/=(const Numeric &num) {
  _buffInst[3].reserve(_buffer.splice().size);

  auto inverse = _buffInst[3].splice(0, size());

  size_t precision = _div.newtonInverse(num._buffer, inverse);

  _div.newtonDiv(_buffer, inverse, _buffer, precision);
  return *this;
}

Numeric Numeric::modExp(const Numeric &base, const Numeric &exp) {
  Numeric value(size());
  _exp.modExp(base._buffer, exp._buffer, _buffer, value._buffer);
  return value;
}

bool Numeric::isSigned() { return _cmp.isSigned(_buffer.splice()); }

} // namespace KCrypt

std::ostream &operator<<(std::ostream &stream, const KCrypt::Numeric &num) {
  stream << num._io.toDecimal(num._buffer.splice());
  return stream;
}

std::istream &operator>>(std::istream &stream, KCrypt::Numeric &num) {
  std::string buffer;
  stream >> buffer;
  num._buffer.reserve(num._io.decSizeInBinary(buffer.size()));
  num._io.toComplement(buffer, num._buffer.splice());
  return stream;
}
