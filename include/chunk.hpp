#pragma once

#include <fstream>

#include "msb_to_lsb.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

class base_chunk {
    public:
        base_chunk(const base_chunk& other): size{other.size} {
            for (int i = 0; i < 4; i++) {
                type[i] = other.type[i];
                crc[i] = other.crc[i];
            }
            data = new byte_t[size];
            for (int i = 0; i < size; i++)
                data[i] = other.data[i];
        }

        base_chunk& operator=(const base_chunk& other)
            { return *this = base_chunk(other); }

        base_chunk(std::ifstream &img, const unsigned int size, const byte_t type[4]): size{size} {
            for (int i = 0; i < 4; i++)
                this->type[i] = type[i];

            // Chunk data
            data = new byte_t[size];
            img.read(reinterpret_cast<char*>(data), size);

            // CRC
            img.read(reinterpret_cast<char*>(crc), 4);
        }

        ~base_chunk()
            { delete[] data; }

        friend std::ostream& operator<<(std::ostream& out, const base_chunk& obj) {
            return out << "=== " << obj.type[0] << obj.type[1] << obj.type[2] << obj.type[3] << " chunk information ===" << std::endl
                       << "not supported" << std::endl;
        }

        friend class PNGfile;

    protected:
        unsigned int size;
        byte_t type[4];
        byte_t* data;
        byte_t crc[4];
};