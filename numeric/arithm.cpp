#define __DEBUG(depth, x)                                                      \
  if (true) {                                                            \
    for (int dpth = 0; dpth < depth; ++dpth) {                                 \
      std::cout << "  ";                                                        \
    }                                                                          \
    std::cout << "at: " << __LINE__ << ",\t" << #x << "\t=\t" << x.size        \
              << ": " << io.getDec(x, *this) << " " << isSigned(x)<<std::endl;             \
  }
#include "arithm.h"
#include "basicio.h"
#include <iostream>

Arithm Arithm::_instance;

Arithm &Arithm::getInstance() { return _instance; }

Arithm::Arithm()
    : _buffer{Buffer<BaseType>::createBuffer(1),
              Buffer<BaseType>::createBuffer(1),
              Buffer<BaseType>::createBuffer(1),
              Buffer<BaseType>::createBuffer(1)}{}

bool Arithm::overflow() { return _wordBuffer.major & 1; }

size_t Arithm::min(size_t a, size_t b) {
  bool min = a < b;
  return a * min + (!min) * b;
}

bool Arithm::equal(SourceBuffer a, SourceBuffer b) {
  BaseType fill = isSigned(a) * BaseType(~0);

  size_t minSize = min(a.size,b.size);

  for (size_t i = 0; i < minSize; ++i) {
    if (a.data[i] != b.data[i]) {
      return false;
    }
  }
  for (size_t i = a.size; i < b.size; ++i) {
    if (b.data[i] != fill) {
      return false;
    }
  }
  for (size_t i = b.size; i < a.size; ++i) {
    if (a.data[i] != fill) {
      return false;
    }
  }
  return true;
}

bool Arithm::greater(SourceBuffer a, SourceBuffer b) {
  size_t i;
  for (i = a.size - 1; i > b.size - 1 && a.data[i] >= b.data[i]; --i) {
  }
  if (i > b.size - 1) {
    return false;
  }
  for (; i > 0; ++i) {
    if (a.data[i - 1] <= b.data[i - 1]) {
      if (a.data[i - 1] < b.data[i - 1]) {
        return false;
      }
      continue;
    }
    return true;
  }
  return false;
}

bool Arithm::less(SourceBuffer a, SourceBuffer b) {
  size_t i;
  for (i = a.size - 1; i > b.size - 1 && a.data[i] < -b.data[i]; --i) {
  }
  if (i > b.size - 1) {
    return false;
  }
  for (; i > 0; ++i) {
    if (a.data[i - 1] >= b.data[i - 1]) {
      if (a.data[i - 1] > b.data[i - 1]) {
        return false;
      }
      continue;
    }
    return true;
  }
  return false;
}

size_t Arithm::leftOffset(SourceBuffer a) {
  int i, j;
  for (i = a.size - 1; i > -1 && a.data[i] == 0; --i) {
  }
  for (j = wordSize - 1; j > -1 && ((a.data[i] >> j) & 1) == 0; --j) {
  }
  return (a.size - i - 1) * wordSize + (wordSize - j - 1);
}

size_t Arithm::rightOffset(SourceBuffer a) {
  int i, j;
  for (i = 0; i < a.size && a.data[i] == 0; ++i) {
  }
  for (j = 0; j < wordSize && ((a.data[i] >> j) & 1) == 0; ++j) {
  }
  return i  * wordSize + j ;
}

void Arithm::addLeft(SourceBuffer a, SourceBuffer b) {
  _wordBuffer.major = 0;
  size_t i = 0;

  for (i = 0; i < b.size; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += b.data[i];
    a.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  BaseType padding = isSigned(b) * BaseType(~0);
  for (; i < a.size; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += padding;
    a.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}

void Arithm::addRight(SourceBuffer a, SourceBuffer b) {
  _wordBuffer.major = 0;
  size_t i = 0;
  for (i = 0; i < b.size; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += b.data[i];
    b.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}

void Arithm::add(SourceBuffer a, BaseType b){
  _wordBuffer.major = b;

  for(size_t i = 0;_wordBuffer.major != 0;++i){
    _wordBuffer.major += a.data[i];
    a.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}

void Arithm::unsignedAddLeft(SourceBuffer a, SourceBuffer b) {
  _wordBuffer.major = 0;
  size_t minSize = min(a.size, b.size);
  size_t i = 0;
  for (i = 0; i < minSize; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += b.data[i];
    a.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  for (; i < a.size && _wordBuffer.major; ++i) {
    _wordBuffer.major += a.data[i];
    a.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}

void Arithm::subLeft(SourceBuffer a, SourceBuffer b) {
  _wordBuffer.major = 1;
  size_t i = 0;

  for (i = 0; i < b.size; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += (~b.data[i]);
    a.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
  BaseType padding = (!isSigned(b)) * BaseType(~0);
  for (; i < a.size; ++i) {
    _wordBuffer.major += a.data[i];
    _wordBuffer.major += padding;
    a.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}

void Arithm::subRight(SourceBuffer a, SourceBuffer b) {
  _wordBuffer.major = 1;
  size_t i = 0;

  for (i = 0; i < b.size; ++i) {
    _wordBuffer.major += (~a.data[i]);
    _wordBuffer.major += b.data[i];
    b.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}

void Arithm::invert(SourceBuffer integer) {
  _wordBuffer.major = 1;
  for (size_t i = 0; i < integer.size; ++i) {
    _wordBuffer.major += ~integer.data[i];
    integer.data[i] = _wordBuffer.minor.low;
    _wordBuffer.major >>= wordSize;
  }
}

bool Arithm::isSigned(SourceBuffer buffer) {
  constexpr size_t signPos = sizeof(BaseType) * 8 - 1;
  return  (buffer.size != 0 ) && (buffer.data[buffer.size - 1] >> signPos);
}

void Arithm::leftShift(SourceBuffer a, SourceBuffer b, size_t shift) {
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);
  size_t complShift = wordSize - minorShift;

  size_t i = a.size - 1;
  size_t j = a.size - majorShift;
  _wordBuffer.major = a.data[j - 1];
  _wordBuffer.major <<= wordSize;
  for (j = j - 1; j > 0; --i, --j) {
    _wordBuffer.major += a.data[j - 1];
    b.data[i] = (_wordBuffer.major >> complShift);
    _wordBuffer.major <<= wordSize;
  }
  b.data[majorShift] = (a.data[j] << minorShift);
  for (; i > 0; --i) {
    b.data[i - 1] = 0;
  }
}
void Arithm::rightShift(SourceBuffer a, SourceBuffer b, size_t shift) {
  size_t majorShift = shift / wordSize;
  size_t minorShift = (shift - majorShift * wordSize);

  size_t i = 0;
  size_t j = majorShift;
  _wordBuffer.major = a.data[j];

  for (j = j + 1; j < a.size; ++i, ++j) {
    _wordBuffer.minor.high = a.data[j];
    b.data[i] = (_wordBuffer.major >> minorShift);
    _wordBuffer.major >>= wordSize;
  }

  b.data[i++] = (_wordBuffer.minor.low >> minorShift);
  for (; i < b.size; ++i) {
    b.data[i] = 0;
  }
}

void Arithm::mul(SourceBuffer b, SourceBuffer a) {
  BufferType mulCache;
  for (int i = a.size - 1, j = 0; i >= 0; --i) {
    mulCache = a.data[i];
    _wordBuffer.major = 0;
    a.data[i] = 0;
    for (j = 0; j + i < a.size & j < b.size; ++j) {
      _wordBuffer.major += a.data[i + j];
      _wordBuffer.major += mulCache * b.data[j];
      a.data[i + j] = _wordBuffer.major;
      _wordBuffer.major >>= wordSize;
    }
    for (; j + i < a.size && _wordBuffer.major != 0; ++j) {
      _wordBuffer.major += a.data[i + j];
      a.data[i + j] = _wordBuffer.major;
      _wordBuffer.major >>= wordSize;
    }
  }
}

void Arithm::div(SourceBuffer a, SourceBuffer b, SourceBuffer c) {
  size_t aShift = a.size * wordSize - leftOffset(a);
  size_t bShift = b.size * wordSize - leftOffset(b);
  size_t shift = aShift - bShift;

  if(shift>>(sizeof(size_t)*8 -1)){
    return;
  }

  BaseBuffer::reserve(_buffer[0], a.size);
  BaseBuffer::reserve(_buffer[1], a.size);
  BaseBuffer aBuf = _buffer[0].splice(0,a.size);
  BaseBuffer bBuf = _buffer[1].splice(0,a.size);

  aBuf.copy(a);
  bBuf.copy(b);

  c.clear();

  leftShift(bBuf, bBuf, shift);

  for (size_t i = 0; i < shift + 2; ++i) {
    leftShift(c, c, 1);
    if (isSigned(aBuf)) {
      addLeft(aBuf, bBuf);
    } else {
      subLeft(aBuf, bBuf);
      c.data[0] += 1;
    }
    rightShift(bBuf, bBuf, 1);
  }

  _wordBuffer.minor.low = wordSize - ((shift + 1) % wordSize);

  (c.data[(shift + 1) / wordSize] <<= _wordBuffer.minor.low) >>=
      _wordBuffer.minor.low;
  c.data[(shift + 1) / wordSize] *= (_wordBuffer.minor.low != 32);
}

// Implementation note:
// Modifies contents of _buffer[0], destroying previous contents
void Arithm::kar(SourceBuffer a, SourceBuffer b, SourceBuffer c) {
  Buffer<BaseType>::reserve(_buffer[0], 4 * a.size);
  Buffer<BaseType>::reserve(_buffer[1], 2 * a.size);
  karIt(a, b, _buffer[1], _buffer[0]);
  c.copy(_buffer[1]);
}
void Arithm::karIt(SourceBuffer a, SourceBuffer b, SourceBuffer c, SourceBuffer d, size_t level) {

  BasicIo &io = BasicIo::getInstance();

  size_t pivot = (a.size >> 1);
  size_t majorPivot = a.size;
  size_t bufferPivot = (a.size << 1);

  if (pivot == 0 || b.size == 0) {
    c.clear();
    if(b.size == 0){
      return;
    } 
    _wordBuffer.major = a.data[0];
    _wordBuffer.major *= b.data[0];
    c.data[0] = _wordBuffer.minor.low;
    c.data[1] = _wordBuffer.minor.high;
    return;
  }

  SourceBuffer la = a.splice(0, pivot);
  SourceBuffer ha = a.splice(pivot, pivot);
  SourceBuffer lb = b.splice(0, min(pivot,b.size));
  SourceBuffer hb = b.splice(lb.size, min(b.size - lb.size,pivot));

  SourceBuffer lc = c.splice(0, majorPivot);
  SourceBuffer hc = c.splice(majorPivot, majorPivot);

  SourceBuffer lBuffer = d.splice(0, pivot);
  SourceBuffer hBuffer = d.splice(pivot, pivot);

  SourceBuffer xBuffer = d.splice(majorPivot, majorPivot);
  SourceBuffer yBuffer = d.splice(majorPivot, majorPivot + 1);
  SourceBuffer rBuffer = d.splice(bufferPivot, bufferPivot);

  karIt(la, lb, lc, rBuffer, level + 1);
  karIt(ha, hb, hc, rBuffer, level + 1);

  lBuffer.copy(ha);
  subLeft(lBuffer, la);
  bool lSign = !overflow();

  hBuffer.copy(lb);
  subLeft(hBuffer, hb);
  bool hSign = !overflow();

  if (hSign) {
    invert(hBuffer);
  }
  if (lSign) {
    invert(lBuffer);
  }

  karIt(lBuffer, hBuffer, xBuffer, rBuffer, level + 1);

  yBuffer.data[yBuffer.size - 1] = 0;

  if (lSign != hSign) {
    invert(yBuffer);
  }

  SourceBuffer c34 = c.splice(pivot, pivot * 3);

  unsignedAddLeft(yBuffer, lc);
  unsignedAddLeft(yBuffer, hc);
  unsignedAddLeft(c34, yBuffer);
}

void Arithm::newtonDiv(SourceBuffer a,SourceBuffer b, SourceBuffer c){
  BasicIo & io = BasicIo::getInstance();
  Buffer<BaseType> inverse = Buffer<BaseType>::createBuffer(a.size);
  Buffer<BaseType> mulBuff = Buffer<BaseType>::createBuffer(a.size * 2);

  inverse.clear();
  mulBuff.clear();

  __DEBUG(0,b);
  newtonInverse(b,inverse,a.size);

  __DEBUG(0,inverse);
  __DEBUG(0,a);
  kar(inverse,a,mulBuff);
  __DEBUG(0,mulBuff);

  rightShift(mulBuff,mulBuff,(a.size * wordSize));
  __DEBUG(0,mulBuff);
  c.copy(mulBuff.splice(0,a.size));
  __DEBUG(0,c);
} 

void Arithm::newtonInverse(SourceBuffer a, SourceBuffer x, size_t inverseSize){ BasicIo & io = BasicIo::getInstance();

  size_t aRightOffset = rightOffset(a);

  x.clear();

  _buffer[2].reserve(_buffer[2],(a.size << 1));
  _buffer[3].reserve(_buffer[3],(a.size << 2));

  size_t aSignificantDigit =  (aRightOffset/wordSize);
  size_t cSignificantDigit = inverseSize  - 2 - aSignificantDigit;

  _wordBuffer.major = 1;
  _wordBuffer.major <<= bufferHighShift;
  _wordBuffer.major /= a.data[aSignificantDigit];
  _wordBuffer.major <<= 1;

  x.data[cSignificantDigit] = _wordBuffer.minor.low;
  x.data[cSignificantDigit+1] = _wordBuffer.minor.high;
  std::cout<<"Significant: "<<cSignificantDigit<<" And + 1"<<std::endl;
  std::cout<<x.data[cSignificantDigit+1]<<" "<<x.data[cSignificantDigit]<<std::endl;
  

  __DEBUG(0,a);
  __DEBUG(0,x);
  for(size_t i = 0;i<100;++i){
      newtonIteration(a,x);
    __DEBUG(0,x);
  }
}

void Arithm::newtonIteration(SourceBuffer a, SourceBuffer x){
  BasicIo & io = BasicIo::getInstance();
  SourceBuffer lBuff = _buffer[2].splice(0,x.size);
  SourceBuffer hBuff = _buffer[2].splice(x.size,x.size);

  //__DEBUG(0,a);
  //__DEBUG(0,x);

  if(a.size>x.size){
    kar(a,x,_buffer[2]);
  }else{
    kar(x,a,_buffer[2]);
  }

  //__DEBUG(0,_buffer[2]);
  invert(_buffer[2]);
  //__DEBUG(0,_buffer[2]);
  add(hBuff, 2);
  //__DEBUG(0,_buffer[2]);

  if(isSigned(_buffer[2])){
     std::cout<<"Error: unexpected signedness"<<std::endl;
  }

  //__DEBUG(0,x)

  kar(_buffer[2],x,_buffer[3]);
  //__DEBUG(0,_buffer[1]);
  //__DEBUG(0,_buffer[3]);
  rightShift(_buffer[3],_buffer[3],x.size* wordSize);
  //__DEBUG(0,_buffer[3]);
  x.copy(_buffer[3]);
}
