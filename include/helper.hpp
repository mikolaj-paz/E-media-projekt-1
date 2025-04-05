#pragma once

#include <algorithm>

#ifndef byte_t
#define byte_t unsigned char
#endif

/// @brief Converts the byte order of the data from Most Significant Byte (MSB) 
///        to Least Significant Byte (LSB).
/// @param ptr Pointer to the data to be converted.
/// @param size The size of the data in bytes.
void MSB_to_LSB(void* ptr, const int size);

/// @brief Converts the byte order of the data from LSB (Least Significant Byte) 
///        to MSB (Most Significant Byte).
/// @param ptr Pointer to the data to be converted.
/// @param size The size of the data in bytes.
void LSB_to_MSB(void* ptr, const int size);

/// @brief Reads an unsigned integer from the byte array.
/// @param ptr Pointer to the byte array from which the unsigned integer will be read.
/// @return The unsigned integer value read from the byte array.
unsigned int read_uint(byte_t* ptr);
