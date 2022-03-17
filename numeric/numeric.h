#ifndef NUMERIC
#define NUMERIC

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "buffer.h"
#include "utils.h"

namespace KCrypt {
using BaseType = uint32_t;

template <class ArthModule, class IoModule> class Numeric {
  IoModule &_ioModule;

  Buffer<BaseType> _buffer;
  static constexpr size_t inSize = 8;
  static constexpr size_t blockSize = 1;

public:
  static Numeric createRandom(size_t size);

  Numeric(const std::string &number);
  Numeric(const char *buffer);

  Numeric(size_t size, BaseType value = 0);

  Numeric();
  Numeric(Numeric &&num);
  Numeric(const Numeric &num);

  size_t size() const;
  void randomize();

  Numeric &operator=(const Numeric &num);
  Numeric &operator=(Numeric &&num);

  Numeric &operator=(BaseType value);

  Numeric operator+(const Numeric &num);
  Numeric operator-(const Numeric &num);
  Numeric operator*(const Numeric &num);
  Numeric operator/(const Numeric &num);
  Numeric operator%(const Numeric &num);

  Numeric &operator+=(const Numeric &num);
  Numeric &operator-=(const Numeric &num);
  Numeric &operator*=(const Numeric &num);
  Numeric &operator/=(const Numeric &num);
  Numeric &operator%=(const Numeric &num);

  bool operator==(const Numeric &num) const;
  bool operator<=(const Numeric &num) const;
  bool operator>=(const Numeric &num) const;
  bool operator<(const Numeric &num) const;
  bool operator>(const Numeric &num) const;

  template <typename A, typename I>
  friend std::ostream &operator<<(std::ostream &stream,
                                  const Numeric<A, I> &num);
  template <typename A, typename I>

  friend std::istream &operator>>(std::istream &stream, Numeric<A, I> &num);

  ~Numeric();
};

template <typename A, typename IO>
Numeric<A, IO>::Numeric()
    : _ioModule(IO::getInstance()), _buffer(Buffer<BaseType>::createBuffer(0)) {
}

template <typename A, typename IO>
Numeric<A, IO>::Numeric(size_t size, BaseType defaultValue)
    : _ioModule(IO::getInstance()),
      _buffer(Buffer<BaseType>::createBuffer(size)) {
  _buffer.data[0] = defaultValue;
}

template <typename A, typename IO>
Numeric<A, IO>::Numeric(Numeric &&num)
    : _ioModule(num._ioModule), _buffer(num._buffer) {}

template <typename A, typename IO>
Numeric<A, IO>::Numeric(const std::string &str)
    : _ioModule(IO::getInstance()), _buffer(_ioModule.parseToComplement(str)) {}

template <typename A, typename IO>
Numeric<A, IO>::Numeric(const Numeric &num)
    : _ioModule(IO::getInstance()), _buffer(num._buffer) {}

template <typename A, typename IO>
Numeric<A, IO> &Numeric<A, IO>::operator=(const Numeric &num) {
  _buffer = num._buffer;
}

template <typename A, typename IO>
Numeric<A, IO> &Numeric<A, IO>::operator=(Numeric &&num) {
  _buffer = std::move(num._buffer);
}

template <typename A, typename IO>
bool Numeric<A, IO>::operator==(const Numeric &num) const {
  return A::equal(_buffer, num._buffer);
}

template <typename A, typename IO>
bool Numeric<A, IO>::operator<(const Numeric &num) const {
  return A::less(_buffer, num._buffer);
}

template <typename A, typename IO>
bool Numeric<A, IO>::operator>(const Numeric &num) const {
  return A::greater(_buffer, num._buffer);
}

template <typename A, typename IO>
std::ostream &operator<<(std::ostream &stream, const Numeric<A, IO> &num) {
  stream << num._ioModule.parseFromComplement(num._buffer);
  return stream;
}

template <typename A, typename IO>
std::istream &operator<<(std::istream &stream, Numeric<A, IO> &num) {
  std::string buffer;
  stream >> buffer;
  num._buffer = num._ioModule.parseToComplement(buffer);
  return stream;
}

template <typename A, typename IO> size_t Numeric<A, IO>::size() const {
  return _buffer.size;
}

template <typename A, typename IO> void Numeric<A, IO>::randomize() {
  //_ioModule.randomize(_buffer);
  Buffer<BaseType>::clear(_buffer);
  _buffer.data[0] = 2137;
}

template <typename A, typename IO>
Numeric<A, IO> Numeric<A, IO>::createRandom(size_t size) {
  Numeric<A, IO> random(size);
  random.randomize();
  return random;
}

template <typename A, typename Io> Numeric<A, Io>::~Numeric() {
  Buffer<BaseType>::releaseBuffer(_buffer);
}

};     // namespace KCrypt
#endif /*NUMERIC*/
