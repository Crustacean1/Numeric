#ifndef NUMERIC
#define NUMERIC

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "buffer.h"
#include "utils.h"
#include "basicio.h"
#include "arithm.h"

namespace KCrypt{
  class Numeric;
}

std::ostream &operator<<(std::ostream &stream, const KCrypt::Numeric &num);
std::istream &operator>>(std::istream &stream, KCrypt::Numeric &num);

namespace KCrypt {
using BaseType = uint32_t;

class Numeric;

class Numeric {

  BasicIo &_ioModule;
  Arithm &_arthModule;
  Buffer<BaseType> _buffer;

public:
  static Numeric createRandom(size_t size,bool isSigned = false);

  Numeric(size_t size, BaseType value = 0);
  Numeric(const std::string &number);
  Numeric(const char *buffer);
  Numeric();
  Numeric(Numeric &&num);
  Numeric(const Numeric &num);

  void randomize(bool isSigned = false);
  void debug();
  size_t size() const;
  bool isSigned();
  std::string toDec();
  std::string toBin();

  /*ASSIGNMENT OPERATORS*/
  Numeric &operator=(const Numeric &num);
  Numeric &operator=(Numeric &&num);
  Numeric &operator=(BaseType value);

  /*NUMERIC VERSION*/
  Numeric operator+(const Numeric &num);
  Numeric operator-(const Numeric &num);
  Numeric operator*(const Numeric &num);
  Numeric operator/(const Numeric &num);
  Numeric operator%(const Numeric &num);
  Numeric operator>>(const Numeric &num);
  Numeric operator<<(const Numeric &num);

  /*IN-BUILT VERSION*/
  Numeric operator+(const BaseType num);
  Numeric operator-(const BaseType num);
  Numeric operator*(const BaseType num);
  Numeric operator/(const BaseType num);
  Numeric operator%(const BaseType num);
  Numeric operator>>(const BaseType num);
  Numeric operator<<(const BaseType num);

  /*NUMERIC VERSION*/
  Numeric &operator+=(const Numeric &num);
  Numeric &operator-=(const Numeric &num);
  Numeric &operator*=(const Numeric &num);
  Numeric &operator/=(const Numeric &num);
  Numeric &operator%=(const Numeric &num);
  Numeric &operator>>=(const Numeric &num);
  Numeric &operator<<=(const Numeric &num);

  /*IN-BUILT VERSION*/
  Numeric &operator+=(BaseType num);
  Numeric &operator-=(BaseType num);
  Numeric &operator*=(BaseType num);
  Numeric &operator/=(BaseType num);
  Numeric &operator%=(BaseType num);
  Numeric &operator>>=(BaseType num);
  Numeric &operator<<=(BaseType num);

  bool operator==(const Numeric &num) const;
  bool operator<=(const Numeric &num) const;
  bool operator>=(const Numeric &num) const;
  bool operator<(const Numeric &num) const;
  bool operator>(const Numeric &num) const;

  operator std::string() const { return _ioModule.getDec(_buffer,_arthModule); }

  friend std::ostream &::operator<<(std::ostream &stream, const Numeric &num);
  friend std::istream &::operator>>(std::istream &stream, Numeric &num);

  ~Numeric();
  };
};     // namespace KCrypt

#endif /*NUMERIC*/
