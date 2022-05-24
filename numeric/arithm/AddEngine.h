#ifndef ADD_ENGING
#define ADD_ENGINE

#include "../Buffer.h"
#include "Comparator.h"

namespace KCrypt{

class AddEngine {

  DoubleBuffer _buffer;
  Comparator & _comparator;

public:
  AddEngine(Comparator & comp);
  //All arguments must be ordered by descending size from left
  void addToLeft(const IntBuffer a, const IntBuffer b);
  void addToRight(const IntBuffer a, const IntBuffer b);
  void addUnsignedToLeft(const IntBuffer a, const IntBuffer b);
  void add(const IntBuffer a, BaseInt b);

  void subFromLeft(const IntBuffer a, const IntBuffer b);
  void subFromRight(const IntBuffer a, const IntBuffer b);
  void invert(const IntBuffer a);

  bool overflow();
};

}
#endif /*ADD_ENGINE*/
