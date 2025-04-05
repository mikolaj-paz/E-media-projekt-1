#pragma once

#include <fstream>

#include "helper.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

class PNGfile;

class base_chunk {
    public:
        base_chunk(const base_chunk& other): size{other.size}, type{other.type} {
            for (int i = 0; i < 4; i++) {
                crc[i] = other.crc[i];
            }
            data = new byte_t[size];
            for (int i = 0; i < size; i++)
                data[i] = other.data[i];           
        }

        base_chunk& operator=(const base_chunk& other)
            { return *this = base_chunk(other); }

        base_chunk(std::ifstream &img, const unsigned int size, const std::string type): size{size}, type{type} {
            // Chunk data
            data = new byte_t[size];
            img.read(reinterpret_cast<char*>(data), size);

            // CRC
            img.read(reinterpret_cast<char*>(crc), 4);
        }

        ~base_chunk()
            { delete[] data; }

        friend std::ostream& operator<<(std::ostream& out, const base_chunk& obj) {
            return out << "=== " << obj.type << " chunk information ===" << std::endl
                       << "not supported" << std::endl;
        }

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const PNGfile& obj);

    protected:
        unsigned int size;
        std::string type;
        byte_t* data;
        byte_t crc[4];
};