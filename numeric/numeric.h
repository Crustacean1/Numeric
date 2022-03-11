#ifndef NUMERIC
#define NUMERIC

#include <cstdint>
#include <cstddef>
#include <cstring>

#include "buffer.h"

namespace KCrypt {
using BaseType = uint32_t;


template<class ArthModule,class MulModule,class IOModule>
class Numeric {
  Buffer<BaseType> _buffer;
  static constexpr size_t minSize = 8;
  static constexpr size_t blockSize = 1;

  void release();
  void initialize(size_t n);

public:
  static Numeric createRandom(size_t size);

  Numeric(const std::string& number);
  Numeric(const char * buffer);

  Numeric(size_t size, BaseType value = 0);

  Numeric();
  Numeric(Numeric &&num);
  Numeric(const Numeric &num);

  size_t size() const;

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

  ~Numeric();
};

template<typename A,typename B,typename IO>
Numeric<A,B,IO>::Numeric(){
  initialize(0); 
}
template<typename A,typename B,typename IO>
Numeric<A,B,IO>::Numeric(size_t size,BaseType defaultValue){
  initialize(size);
  _buffer.data[0] = defaultValue;
}

template<typename A,typename B,typename IO>
Numeric<A,B,IO>::Numeric(Numeric && num){
  _buffer.data = num._buffer.data;
  _buffer.size = num._buffer.size;

  num._buffer.data = nullptr;
}

template<typename A,typename M,typename IO>
Numeric<A,M,IO>::Numeric(const std::string & str){
  IO::parseToComplement(_buffer,str);
}

template<typename A,typename M,typename IO>
Numeric<A,M,IO>::Numeric(const Numeric & num){
  initialize(num.size());
  memcpy(_buffer.data,num._buffer.data,num._buffer.size);
}

template<typename A,typename B,typename IO>
size_t Numeric<A,B,IO>::size()const{
  return _buffer.size;
}

template<typename A,typename B,typename IO>
Numeric<A,B,IO> Numeric<A,B,IO>::createRandom(size_t size){
  Numeric<A,B,IO> random(size);
  IO::randomize(random._buffer);
  return random;
}


template<typename A,typename M,typename IO>
Numeric<A,M,IO>::~Numeric(){
  release();
}

template<typename A,typename M,typename IO>
Numeric<A,M,IO>& Numeric<A,M,IO>::operator=(const Numeric & num){
  release();
  _buffer.data = memcpy(num._buffer.data,num._buffer.data,std::min(num._buffer.size,_buffer.size));
  _buffer.size = num._data.size;
}

template<typename A,typename M,typename IO>
Numeric<A,M,IO>& Numeric<A,M,IO>::operator=(Numeric && num){
  release();
  _buffer.data = num._data.data;
  _buffer.size = num._data.size;
  num.release();
}

template<typename A,typename M,typename IO>
bool Numeric<A,M,IO>::operator==(const Numeric & num)const{
  return A::equal(_buffer,num._buffer);
}

template<typename A,typename M,typename IO>
bool  Numeric<A,M,IO>::operator<(const Numeric & num)const{
  return A::less(_buffer,num._buffer);
}

template<typename A,typename M,typename IO>
bool Numeric<A,M,IO>::operator>(const Numeric & num)const{
  return A::greater(_buffer,num._buffer);
}

template<typename A,typename M,typename IO>
void Numeric<A,M,IO>::release(){
  if(_buffer.data != nullptr){
    delete[] _buffer.data;
  }
}

template<typename A,typename M,typename IO>
void Numeric<A,M,IO>::initialize(size_t n)
{
  size_t newSize = 1;
  while(newSize<n && newSize<minSize){
    newSize<<=blockSize;
  }
  _buffer.data = new BaseType[newSize];
  _buffer.size = newSize;
}
}; // namespace KCrypt
#endif /*NUMERIC*/