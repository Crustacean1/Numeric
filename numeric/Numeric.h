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

// Notes:
// - Has constant size thorough lifetime
// - Results of operations are the same size as the source
// - Thread safe (TODO)

class Numeric {

  BufferInstance &_buffInst;

  BasicIo _io;

  CompEngine _cmp;
  AddEngine _add;
  MulEngine _mul;
  DivEngine _div;
  ExpEngine _exp;

  Buffer _buffer;

  Numeric &orderedMul(const BufferView &a, const BufferView &b, Buffer &c);

public:
  Numeric(size_t size, BufferView::BaseInt value = 0);
  Numeric(const std::string &number);

  Numeric(Buffer &&buffer);
  Numeric(const BufferView &buffer);

  Numeric();
  Numeric(Numeric &&num);
  Numeric(const Numeric &num);

  void debug();
  size_t size() const;
  bool isSigned();
  std::string toDec();
  std::string toBin();

  Numeric modExp(const Numeric & base, const Numeric & exp);
  Numeric gcd(const Numeric & num);
  bool testMillerRabin(const Numeric & witness);

  /*ASSIGNMENT OPERATORS*/
  Numeric &operator=(const Numeric &num);
  Numeric &operator=(Numeric &&num);
  Numeric &operator=(BufferView::BaseInt value);

  /*NUMERIC VERSION*/
  Numeric operator+(const Numeric &num) const;
  Numeric operator-(const Numeric &num) const;
  Numeric operator*(const Numeric &num) const;
  Numeric operator/(const Numeric &num) const;
  Numeric operator%(const Numeric &num) const;
  Numeric operator>>(const Numeric &num) const;
  Numeric operator<<(const Numeric &num) const;

  /*BASE VERSION*/
  Numeric operator+(const BufferView::BaseInt num) const;
  Numeric operator-(const BufferView::BaseInt num) const;
  Numeric operator*(const BufferView::BaseInt num) const;
  Numeric operator/(const BufferView::BaseInt num) const;
  Numeric operator%(const BufferView::BaseInt num) const;
  Numeric operator>>(const BufferView::BaseInt num) const;
  Numeric operator<<(const BufferView::BaseInt num) const;

  /*NUMERIC VERSION*/
  Numeric &operator+=(const Numeric &num);
  Numeric &operator-=(const Numeric &num);
  Numeric &operator*=(const Numeric &num);
  Numeric &operator/=(const Numeric &num);
  Numeric &operator%=(const Numeric &num);
  Numeric &operator>>=(const Numeric &num);
  Numeric &operator<<=(const Numeric &num);

  /*BASE VERSION*/
  Numeric &operator+=(BufferView::BaseInt num);
  Numeric &operator-=(BufferView::BaseInt num);
  Numeric &operator*=(BufferView::BaseInt num);
  Numeric &operator/=(BufferView::BaseInt num);
  Numeric &operator%=(BufferView::BaseInt num);
  Numeric &operator>>=(BufferView::BaseInt num);
  Numeric &operator<<=(BufferView::BaseInt num);

  bool operator==(const Numeric &num) const;
  bool operator<=(const Numeric &num) const;
  bool operator>=(const Numeric &num) const;
  bool operator<(const Numeric &num) const;
  bool operator>(const Numeric &num) const;

  bool operator==(BufferView::BaseInt num) const;
  bool operator<=(BufferView::BaseInt num) const;
  bool operator>=(BufferView::BaseInt num) const;
  bool operator<(BufferView::BaseInt num) const;
  bool operator>(BufferView::BaseInt num) const;

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
