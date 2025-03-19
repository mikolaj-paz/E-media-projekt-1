#pragma once

#include <ostream>
#include <fstream>
#include <string>
#include <map>

#include "chunk.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

class tIME : public base_chunk {
    public:
        struct UTC {
            UTC(const byte_t byteArray[7]);

            int year;
            int month;
            int day;
            int hour;
            int minute;
            int second;

            friend std::ostream& operator<<(std::ostream& out, const UTC& obj);
        };

        tIME(std::ifstream &img, const unsigned int size, const std::string type):
            base_chunk(img, size, type),
            timeStamp(data)
        {}

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const tIME& obj);
        
    private:
        UTC timeStamp;
};
    
class tEXt : public base_chunk {
    public:
        tEXt(std::ifstream &img, const unsigned int size, const std::string type);
        
        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const tEXt& obj);
    
    private:
        std::string keyword;
        std::string text;
};

class bKGD : public base_chunk {
    public:
        bKGD(std::ifstream &img, const unsigned int size, const std::string type, const byte_t& colorType, const byte_t& bitDepth);

        ~bKGD() {
            if (greyscale) delete greyscale;
            if (truecolor) delete[] truecolor;
            if (paletteIndex) delete paletteIndex;
        }

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const bKGD& obj);
    
    private:
        short* greyscale = nullptr;
        short* truecolor = nullptr;
        byte_t* paletteIndex = nullptr;
        
        static const std::map<byte_t, short> bitDepthMask;
};

class gAMA : public base_chunk {
    public:
        gAMA(std::ifstream &img, const unsigned int size, const std::string type): base_chunk(img, size, type) {
            gamma = *(reinterpret_cast<unsigned int*>(data));
            MSB_to_LSB(&gamma, 4);
        }

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const gAMA& obj);

    private:
        unsigned int gamma;
};

class pHYs : public base_chunk {
    public:
        pHYs(std::ifstream &img, const unsigned int size, const std::string type);

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const pHYs& obj);

    private:
        unsigned int pixX;
        unsigned int pixY;
        byte_t unit;
};

class cHRM : public base_chunk {
    public:
        cHRM(std::ifstream &img, const unsigned int size, const std::string type);

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const cHRM& obj);
    
    private:
        unsigned int wpx;
        unsigned int wpy;
        unsigned int rx;
        unsigned int ry;
        unsigned int gx;
        unsigned int gy;
        unsigned int bx;
        unsigned int by;

        unsigned int cast_to_uint(byte_t* ptr) {
            unsigned int tmp = *(reinterpret_cast<unsigned int*>(ptr));
            MSB_to_LSB(&tmp, 4);
            return tmp;
        }
};