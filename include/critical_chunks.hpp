#pragma once

#include <ostream>
#include <fstream>

#include "chunk.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

class IHDR : public base_chunk {
    public:
        IHDR(std::ifstream &img, const unsigned int size, const byte_t type[4]);

        friend std::ostream& operator<<(std::ostream& out, const IHDR& obj);
        friend class PNGfile;
            
    private:
        unsigned int width;
        unsigned int height;
        
        byte_t bit_depth;
        byte_t color_type;
        byte_t compression_method;
        byte_t filter_method;
        byte_t interlace_method;
};

class IDAT : public base_chunk {
    public:
        IDAT(std::ifstream &img, const unsigned int size, const byte_t type[4]): base_chunk(img, size, type) {}

        friend std::ostream& operator<<(std::ostream& out, const IDAT& obj);
        friend class PNGfile;
};

class PLTE : public base_chunk {
    public:
        PLTE(std::ifstream &img, const unsigned int size, const byte_t type[4]): base_chunk(img, size, type) {}

        friend std::ostream& operator<<(std::ostream& out, const PLTE& obj);
        friend class PNGfile;
};