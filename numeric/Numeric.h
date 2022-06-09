#ifndef NUMERIC
#define NUMERIC

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <tuple>

#include "Buffer/Buffer.h"
#include "ArithmFacade.h"

namespace KCrypt {


class Numeric;
} // namespace KCrypt

std::ostream &operator<<(std::ostream &stream, const KCrypt::Numeric &num);
std::istream &operator>>(std::istream &stream, KCrypt::Numeric &num);

namespace KCrypt {

class Numeric;

// Notes:
// - [x] Has constant size thorough lifetime
// - [x] Results of operations are of the same size as the source
// - [ ]Thread safe (TODO)

class Numeric {

  ArithmFacade & _arithm;

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

  Buffer & getBuffer();
  size_t size() const;

  std::string toDec();
  std::string toBin();

  bool isSigned();
  Numeric & inverse();
  Numeric & abs();

  Numeric modExp(const Numeric & base, const Numeric & exp);
  std::tuple<Numeric,Numeric> extGcd(const Numeric & num);

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
