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

std::string Numeric::toDec() { return _arithm.writeToString(_buffer); }
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

Numeric &Numeric::operator+=(const Numeric &num) { return *this; }

Numeric &Numeric::operator-=(const Numeric &num) { return *this; }

Numeric &Numeric::operator-=(const BufferView::BaseInt b) {
  _add.sub(_buffer, b);
  return *this;
}

Numeric &Numeric::operator<<=(const Numeric &num) { return *this; }

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
                             Buffer &c) {}

Numeric &Numeric::operator*=(const Numeric &num) {
  if (size() > num.size()) {
    return orderedMul(_buffer.splice(), num._buffer.splice(), _buffInst[3]);
  }
  return orderedMul(num._buffer.splice(), _buffer.splice(), _buffInst[3]);
}

Numeric Numeric::operator/(const Numeric &num) const {}

Numeric &Numeric::operator/=(const Numeric &num) {}

Numeric Numeric::modExp(const Numeric &base, const Numeric &exp) {}

std::tuple<Numeric, Numeric> Numeric::extGcd(const Numeric &num) {}

Numeric &Numeric::inverse() {}
Numeric &Numeric::abs() {}

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
