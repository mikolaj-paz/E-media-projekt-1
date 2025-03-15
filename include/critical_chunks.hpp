#pragma once

#include <ostream>
#include <fstream>
#include <string>

#include "chunk.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

class IHDR : public base_chunk {
    public:
        IHDR(std::ifstream &img, const unsigned int size, const std::string& type);

        friend std::ostream& operator<<(std::ostream& out, const IHDR& obj);
        friend class PNGfile;
            
    private:
        unsigned int width;
        unsigned int height;
        
        byte_t bitDepth;
        byte_t colorType;
        byte_t compressionMethod;
        byte_t filterMethod;
        byte_t interlaceMethod;
};

class IDAT : public base_chunk {
    public:
        IDAT(std::ifstream &img, const unsigned int size, const std::string& type): base_chunk(img, size, type) {}

        friend std::ostream& operator<<(std::ostream& out, const IDAT& obj);
        friend class PNGfile;
};

class PLTE : public base_chunk {
    public:
        PLTE(std::ifstream &img, const unsigned int size, const std::string& type): base_chunk(img, size, type) {}

        friend std::ostream& operator<<(std::ostream& out, const PLTE& obj);
        friend class PNGfile;
};