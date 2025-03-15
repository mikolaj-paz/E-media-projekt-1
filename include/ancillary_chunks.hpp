#pragma once

#include <ostream>
#include <fstream>
#include <string>

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

        tIME(std::ifstream &img, const unsigned int size, const byte_t type[4]):
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
            tEXt(std::ifstream &img, const unsigned int size, const byte_t type[4]);
            
            friend class PNGfile;
            friend std::ostream& operator<<(std::ostream& out, const tEXt& obj);
        
        private:
            std::string keyword;
            std::string text;
};