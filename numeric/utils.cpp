#include "utils.h"

size_t KUtils::min(size_t a,size_t b){
  auto comp = a<b;
  return comp*a + (!comp)*b;
}
size_t KUtils::max(size_t a,size_t b){
  auto comp = a>b;
  return comp*a + (!comp)*b;
}
