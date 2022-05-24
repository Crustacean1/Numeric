#include "Utils.h"

size_t K::min(size_t a,size_t b){
  auto comp = a<b;
  return comp*a + (!comp)*b;
}
size_t K::max(size_t a,size_t b){
  auto comp = a>b;
  return comp*a + (!comp)*b;
}
