#pragma once

#include <istream>
#include <ostream>
#include <fstream>

#ifndef byte_t
#define byte_t unsigned char
#endif

class IHDR {
    public:
        IHDR(std::ifstream &img)
            { read(img); }

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

        void read(std::ifstream &img);
};