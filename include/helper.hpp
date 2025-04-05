#pragma once

#include <algorithm>

#ifndef byte_t
#define byte_t unsigned char
#endif

void MSB_to_LSB(void* ptr, const int size);

void LSB_to_MSB(void* ptr, const int size);

unsigned int read_uint(byte_t* ptr);
