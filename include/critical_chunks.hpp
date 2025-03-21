#pragma once

#include <ostream>
#include <fstream>
#include <string>
#include <vector>

#include "chunk.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

class IHDR : public base_chunk {
    public:
        IHDR(std::ifstream &img, const unsigned int size, const std::string& type);

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const IHDR& obj);
            
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

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const IDAT& obj);
};

class PLTE : public base_chunk {
    public:
        PLTE(std::ifstream &img, const unsigned int size, const std::string& type);

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const PLTE& obj);

    private:
        struct rgb_entry {
            rgb_entry(const byte_t& red, const byte_t& green, const byte_t& blue):
                red{red}, green{green}, blue{blue} {}

            byte_t red;
            byte_t green;
            byte_t blue;
        };

        std::vector<rgb_entry> entries;
    
    public:
        friend std::ostream& operator<<(std::ostream& out, const rgb_entry& obj);
};

class IEND : public base_chunk {
    public:
        IEND(std::ifstream &img, const unsigned int size, const std::string& type): base_chunk(img, size, type) {}

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const IEND& obj);
};