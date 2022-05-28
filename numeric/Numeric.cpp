#include "Numeric.h"

namespace KCrypt {

Numeric::Numeric() : Numeric(Buffer(1)) {}

Numeric::Numeric(const BufferView &buffer)
    : _buffInst(BufferInstance::getInstance()), _cmp(), _add(_cmp),
      _io(_cmp, _add), _mul(_cmp, _add, _buffInst[0]),
      _div(_cmp, _add, _mul, _buffInst[1], _buffInst[2]),
      _exp(_cmp, _add, _mul, _div), _bufferHandle(buffer.size) {
  _bufferHandle.splice().copy(buffer);
}

Numeric::Numeric(Buffer &&bufferHandle)
    : _buffInst(BufferInstance::getInstance()), _cmp(), _add(_cmp),
      _io(_cmp, _add), _mul(_cmp, _add, _buffInst[0]),
      _div(_cmp, _add, _mul, _buffInst[1], _buffInst[2]),
      _exp(_cmp, _add, _mul, _div), _bufferHandle(std::move(bufferHandle)) {}

Numeric::Numeric(size_t size, BufferView::BaseInt defaultValue)
    : Numeric(Buffer(size)) {
  _bufferHandle.splice().clear();
  _bufferHandle.splice().data[0] = defaultValue;
}

Numeric::Numeric(const Numeric &num) : Numeric(num._bufferHandle.splice()) {}

Numeric::Numeric(Numeric &&num) : Numeric(std::move(num._bufferHandle)) {}

Numeric::Numeric(const std::string &str) : Numeric() {
  _bufferHandle.reserve(_io.decSizeInBinary(str.size()));
  _io.toComplement(str, _bufferHandle.splice());
}

Numeric &Numeric::operator=(const Numeric &num) {
  _bufferHandle.splice().copy(num._bufferHandle.splice());
  return *this;
}

bool Numeric::operator==(const Numeric &num) const {
  if (_bufferHandle.splice().size > num._bufferHandle.splice().size) {
    return _cmp.equal(_bufferHandle.splice(), num._bufferHandle.splice());
  }
  return _cmp.equal(num._bufferHandle.splice(), _bufferHandle.splice());
}

bool Numeric::operator<(const Numeric &num) const {
  if (_bufferHandle.size > num._bufferHandle.size) {
    return _cmp.lesser(_bufferHandle.splice(), num._bufferHandle.splice());
  }
  return !_cmp.lesser(num._bufferHandle.splice(), _bufferHandle.splice());
}

bool Numeric::operator>(const Numeric &num) const {
  if (_bufferHandle.size > num._bufferHandle.size) {
    return _cmp.greater(_bufferHandle.splice(), num._bufferHandle.splice());
  }
  return !_cmp.greater(num._bufferHandle.splice(), _bufferHandle.splice());
}

size_t Numeric::size() const { return _bufferHandle.splice().size; }

Numeric::~Numeric() {}

std::string Numeric::toDec() { return _io.toDecimal(_bufferHandle.splice()); }
std::string Numeric::toBin() { return _io.toBinary(_bufferHandle.splice()); }

Numeric Numeric::operator+(const Numeric &num) const {
  Numeric sum(_bufferHandle.splice().size);
  sum._bufferHandle.splice().copy(_bufferHandle.splice());

  if (sum._bufferHandle.splice().size < num._bufferHandle.splice().size) {
    sum._add.addToRight(num._bufferHandle.splice(), sum._bufferHandle.splice());
    return sum;
  }
  sum._add.addToLeft(sum._bufferHandle.splice(), num._bufferHandle.splice());
  return sum;
}

Numeric Numeric::operator-(const Numeric &num) const {
  Numeric sum(_bufferHandle.splice().size);
  sum._bufferHandle.splice().copy(_bufferHandle.splice());

  if (sum._bufferHandle.splice().size < num._bufferHandle.splice().size) {
    sum._add.subFromRight(num._bufferHandle.splice(),
                          sum._bufferHandle.splice());
    return sum;
  }

  sum._add.subFromLeft(sum._bufferHandle.splice(), num._bufferHandle.splice());
  return sum;
}

Numeric &Numeric::operator+=(const Numeric &num) {
  if (_bufferHandle.splice().size < num._bufferHandle.splice().size) {
    _add.addToRight(num._bufferHandle.splice(), _bufferHandle.splice());
    return *this;
  }
  _add.addToLeft(_bufferHandle.splice(), num._bufferHandle.splice());
  return *this;
}

Numeric &Numeric::operator-=(const Numeric &num) {
  if (_bufferHandle.splice().size < num._bufferHandle.splice().size) {
    _add.subFromRight(num._bufferHandle.splice(), _bufferHandle.splice());
    return *this;
  }
  _add.subFromLeft(_bufferHandle.splice(), num._bufferHandle.splice());
  return *this;
}

Numeric &Numeric::operator<<=(const Numeric &num) {
  _add.leftShift(_bufferHandle.splice(), _bufferHandle.splice(),
                 num._bufferHandle.splice().data[0]);
  return *this;
}

Numeric &Numeric::operator>>=(const Numeric &num) {
  _add.rightShift(_bufferHandle.splice(), _bufferHandle.splice(),
                  num._bufferHandle.splice().data[0]);
  return *this;
}

Numeric Numeric::operator*(const Numeric &num) const {
  Numeric result(size());

  if (size() > num.size()) {
    return result.orderedMul(result._bufferHandle.splice(),
                             num._bufferHandle.splice(), _buffInst[3]);
  }
  return result.orderedMul(result._bufferHandle.splice(),
                           num._bufferHandle.splice(), _buffInst[3]);
}

Numeric &Numeric::orderedMul(const BufferView &a, const BufferView &b,
                             Buffer &c) {
  c.reserve(a.size * 2);
  auto result = c.splice(0, a.size * 2);
  _mul.kar(a, b, c.splice(0, a.size * 2));
  _bufferHandle.splice().copy(result);
  return *this;
}

Numeric &Numeric::operator*=(const Numeric &num) {
  if (size() > num.size()) {
    return orderedMul(_bufferHandle.splice(), num._bufferHandle.splice(),
                      _buffInst[3]);
  }
  return orderedMul(num._bufferHandle.splice(), _bufferHandle.splice(),
                    _buffInst[3]);
}

Numeric Numeric::operator/(const Numeric &num) const {
  Numeric result(size());
  result._div.div(_bufferHandle.splice(), num._bufferHandle.splice(),
                  result._bufferHandle.splice());
  return result;
}

Numeric &Numeric::operator/=(const Numeric &num) {
  _buffInst[3].reserve(_bufferHandle.splice().size);

  auto inverse = _buffInst[3].splice(0, size());

  size_t precision = _div.newtonInverse(num._bufferHandle.splice(), inverse);

  _div.newtonDiv(_bufferHandle.splice(), inverse, _bufferHandle.splice(),
                 precision);
  return *this;
}

bool Numeric::isSigned() { return _cmp.isSigned(_bufferHandle.splice()); }

} // namespace KCrypt

std::ostream &operator<<(std::ostream &stream, const KCrypt::Numeric &num) {
  stream << num._io.toDecimal(num._bufferHandle.splice());
  return stream;
}

std::istream &operator>>(std::istream &stream, KCrypt::Numeric &num) {
  std::string buffer;
  stream >> buffer;
  num._bufferHandle.reserve(num._io.decSizeInBinary(buffer.size()));
  num._io.toComplement(buffer, num._bufferHandle.splice());
  return stream;
}
