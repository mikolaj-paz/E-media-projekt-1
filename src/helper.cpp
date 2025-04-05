#include <helper.hpp>

void MSB_to_LSB(void* ptr, const int size) {
    char* istart = reinterpret_cast<char*>(ptr);
    char* iend = istart + size;
    std::reverse(istart, iend);
}

void LSB_to_MSB(void* ptr, const int size)
    { MSB_to_LSB(ptr, size); }

unsigned int read_uint(byte_t* ptr) {
    unsigned int tmp = *(reinterpret_cast<unsigned int*>(ptr));
    MSB_to_LSB(&tmp, 4);
    return tmp;
}