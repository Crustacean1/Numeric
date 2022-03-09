module;
#include <cstdint>
#include <cstddef>
export module numeric;

namespace KCrypt {
using BaseType = uint32_t;

export class Numeric {
  BaseType *_data;
  size_t _size;

public:
  Numeric();
  Numeric(const char * str);
  Numeric(size_t size, BaseType value = 0);
  Numeric(Numeric &&num);
  Numeric(const Numeric &num);
  ~Numeric();

  Numeric &operator=(const Numeric &num);
  Numeric &operator=(Numeric &&num);

  Numeric &operator=(BaseType value);

  Numeric operator+(const Numeric &num);
  Numeric operator-(const Numeric &num);

  Numeric &operator+=(const Numeric &num);
  Numeric &operator-=(const Numeric &num);
};
}; // namespace KCrypt