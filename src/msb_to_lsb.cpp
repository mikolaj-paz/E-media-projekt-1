#include <msb_to_lsb.hpp>

void MSB_to_LSB(void* ptr, const int size) {
    char* istart = reinterpret_cast<char*>(ptr);
    char* iend = istart + size;
    std::reverse(istart, iend);
}

void LSB_to_MSB(void* ptr, const int size)
    { MSB_to_LSB(ptr, size); }