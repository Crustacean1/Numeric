#include <iostream>

#include "Numeric.h"

#include "Arithm/AddEngine.h"
#include "Arithm/CompEngine.h"
#include "Arithm/DivEngine.h"
#include "Arithm/ExpEngine.h"
#include "Arithm/GcdEngine.h"
#include "Arithm/MulEngine.h"

namespace KCrypt {

Numeric::Numeric() : Numeric(Buffer(1)) {}

Numeric::Numeric(const BufferView &buffer)
    : _arithm(ArithmFacade::getInstance(0)), _buffer(buffer.size) {
  _buffer.splice().copy(buffer);
}

Numeric::Numeric(Buffer &&bufferHandle)
    : _arithm(ArithmFacade::getInstance(0)), _buffer(std::move(bufferHandle)) {}

Numeric::Numeric(size_t size, BufferView::BaseInt defaultValue)
    : Numeric(Buffer(size)) {
  _buffer.splice().clear();
  _buffer.splice().data[0] = defaultValue;
}

Numeric::Numeric(const Numeric &num) : Numeric(num._buffer.splice()) {}

Numeric::Numeric(Numeric &&num) : Numeric(std::move(num._buffer)) {}

Numeric::Numeric(const std::string &str) : Numeric() {
  _arithm.readFromString(str, _buffer);
}

Numeric &Numeric::operator=(const Numeric &num) {
  _buffer.splice().copy(num._buffer.splice());
  return *this;
}

bool Numeric::operator==(const Numeric &num) const {
  return _arithm.equal(_buffer, num._buffer);
}

bool Numeric::operator<(const Numeric &num) const {
  return _arithm.lesser(_buffer, num._buffer);
}

bool Numeric::operator>(const Numeric &num) const {
  return _arithm.greater(_buffer, num._buffer);
}

size_t Numeric::size() const { return _buffer.splice().size; }

Numeric::~Numeric() {}

std::string Numeric::toDec() { return _arithm.writeDecimal(_buffer); }
std::string Numeric::toBin() { return _arithm.writeBinary(_buffer); }

Numeric Numeric::operator+(const Numeric &num) const {
  Numeric sum(*this);
  _arithm.add(num._buffer, sum._buffer);
  return sum;
}

Numeric Numeric::operator-(const Numeric &num) const {
  Numeric sub(*this);
  _arithm.subtract(num._buffer, sub._buffer);
  return sub;
}

Numeric &Numeric::operator+=(const Numeric &num) {
  _arithm.add(num._buffer, _buffer);
  return *this;
}

Numeric &Numeric::operator-=(const Numeric &num) {
  _arithm.subtract(num._buffer, _buffer);
  return *this;
}

Numeric &Numeric::operator-=(BufferView::BaseInt num) {
  _arithm.subtract(num, _buffer);
  return *this;
}

Numeric &Numeric::operator<<=(const Numeric &num) { return *this; }

Numeric &Numeric::operator>>=(const Numeric &num) {
  _arithm.rightShift(_buffer, num._buffer.data[0]);
  return *this;
}

Numeric Numeric::operator*(const Numeric &num) const {
  Numeric result(size());

  if (size() > num.size()) {
    _arithm.multiply(_buffer, num._buffer, result._buffer);
    return result;
  }
  _arithm.multiply(num._buffer, num._buffer, result._buffer);
  return result;
}

Numeric &Numeric::operator*=(const Numeric &num) {
  if (size() > num.size()) {
    _arithm.multiply(_buffer, num._buffer, _buffer);
    return *this;
  }
  _arithm.multiply(num._buffer, num._buffer, _buffer);
  return *this;
}

Numeric Numeric::operator/(const Numeric &num) const {
  Numeric result(size());

  _arithm.divide(_buffer, num._buffer, result._buffer);
  return result;
}

Numeric &Numeric::operator/=(const Numeric &num) {
  _arithm.divide(_buffer, num._buffer, _buffer);
  return *this;
}

Numeric Numeric::modExp(const Numeric &base, const Numeric &exp) {
  Numeric result(size());
  _arithm.modExp(base._buffer, exp._buffer, _buffer, result._buffer);
  return result;
}

std::tuple<Numeric, Numeric> Numeric::extGcd(const Numeric &num) {
  Numeric ext1(size());
  Numeric ext2(num.size());
  _arithm.extGcd(num._buffer, _buffer, ext1._buffer, ext2._buffer);
  return std::make_tuple(ext1, ext2);
}

Numeric &Numeric::inverse() {
  _arithm.invert(_buffer);
  return *this;
}

Numeric &Numeric::abs() {
  if (_arithm.isSigned(_buffer)) {
    _arithm.invert(_buffer);
  }
  return *this;
}

bool Numeric::isSigned() { return _arithm.isSigned(_buffer.splice()); }

} // namespace KCrypt

std::ostream &operator<<(std::ostream &stream, const KCrypt::Numeric &num) {
  stream << num._arithm.writeDecimal(num._buffer.splice());
  return stream;
}

std::istream &operator>>(std::istream &stream, KCrypt::Numeric &num) {
  std::string buffer;
  stream >> buffer;
  num._arithm.readFromString(buffer, num._buffer);
  return stream;
}
