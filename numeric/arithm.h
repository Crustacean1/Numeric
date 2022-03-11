#ifndef ARITHM
#define ARITHM

#include "buffer.h"

class Arithm{
    static size_t min(size_t a,size_t b);
    public:
    static bool equal(const Buffer<BaseType>& a,const Buffer<BaseType>& b);
};

#endif /*ARITHM*/