#include "basicio.h"

std::default_random_engine BasicIo::_engine(2137);

void BasicIo::randomize(Buffer<BaseType> & buffer){
    std::uniform_int_distribution<BaseType> dist(0,-1);
    for(size_t i = 0;i<buffer.size;++i){
        buffer.data[i] = dist(_engine);
    }
}
void BasicIo::parseToComplement(Buffer<BaseType> & buffer, const std::string & str)
{
    buffer.data[0] = 2137;//binary algo, todo
}