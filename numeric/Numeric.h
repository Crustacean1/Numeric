#ifndef NUMERIC
#define NUMERIC

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "BasicIo.h"
#include "Buffer/Buffer.h"
#include "Buffer/BufferInstance.h"

#include "Arithm/AddEngine.h"
#include "Arithm/CompEngine.h"
#include "Arithm/DivEngine.h"
#include "Arithm/ExpEngine.h"
#include "Arithm/MulEngine.h"

namespace KCrypt {
class Numeric;
} // namespace KCrypt

std::ostream &operator<<(std::ostream &stream, const KCrypt::Numeric &num);
std::istream &operator>>(std::istream &stream, KCrypt::Numeric &num);

namespace KCrypt {

class Numeric;

class Numeric {

  BufferInstance &_buffInst;

  BasicIo _io;

  CompEngine _cmp;
  AddEngine _add;
  MulEngine _mul;
  DivEngine _div;
  ExpEngine _exp;

  Buffer<BaseInt> _buffer;

  Numeric &orderedMul(const IntBuffer &a, const IntBuffer &b, IntBuffer &c);

public:
  Numeric(size_t size, BaseInt value = 0);
  Numeric(const std::string &number);

  Numeric(const Buffer<BaseInt> &buffer);
  Numeric(Buffer<BaseInt> &&buffer);

  Numeric();
  Numeric(Numeric &&num);
  Numeric(const Numeric &num);

  void debug();
  size_t size() const;
  bool isSigned();
  std::string toDec();
  std::string toBin();

  /*ASSIGNMENT OPERATORS*/
  Numeric &operator=(const Numeric &num);
  Numeric &operator=(Numeric &&num);
  Numeric &operator=(BaseInt value);

  /*NUMERIC VERSION*/
  Numeric operator+(const Numeric &num) const;
  Numeric operator-(const Numeric &num) const;
  Numeric operator*(const Numeric &num) const;
  Numeric operator/(const Numeric &num) const;
  Numeric operator%(const Numeric &num) const;
  Numeric operator>>(const Numeric &num) const;
  Numeric operator<<(const Numeric &num) const;

  /*BASE VERSION*/
  Numeric operator+(const BaseInt num) const;
  Numeric operator-(const BaseInt num) const;
  Numeric operator*(const BaseInt num) const;
  Numeric operator/(const BaseInt num) const;
  Numeric operator%(const BaseInt num) const;
  Numeric operator>>(const BaseInt num) const;
  Numeric operator<<(const BaseInt num) const;

  /*NUMERIC VERSION*/
  Numeric &operator+=(const Numeric &num);
  Numeric &operator-=(const Numeric &num);
  Numeric &operator*=(const Numeric &num);
  Numeric &operator/=(const Numeric &num);
  Numeric &operator%=(const Numeric &num);
  Numeric &operator>>=(const Numeric &num);
  Numeric &operator<<=(const Numeric &num);

  /*BASE VERSION*/
  Numeric &operator+=(BaseInt num);
  Numeric &operator-=(BaseInt num);
  Numeric &operator*=(BaseInt num);
  Numeric &operator/=(BaseInt num);
  Numeric &operator%=(BaseInt num);
  Numeric &operator>>=(BaseInt num);
  Numeric &operator<<=(BaseInt num);

  bool operator==(const Numeric &num) const;
  bool operator<=(const Numeric &num) const;
  bool operator>=(const Numeric &num) const;
  bool operator<(const Numeric &num) const;
  bool operator>(const Numeric &num) const;

  bool operator==(BaseInt num) const;
  bool operator<=(BaseInt num) const;
  bool operator>=(BaseInt num) const;
  bool operator<(BaseInt num) const;
  bool operator>(BaseInt num) const;

  operator std::string() const {
    // return _ioModule.getDec(_buffer, _arthModule);
    return "";
  }

  friend std::ostream & ::operator<<(std::ostream &stream, const Numeric &num);
  friend std::istream & ::operator>>(std::istream &stream, Numeric &num);

  ~Numeric();
};
}; // namespace KCrypt

#endif /*NUMERIC*/
