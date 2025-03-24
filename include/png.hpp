#pragma once

#include <exception>
#include <cstring>
#include <fstream>
#include <vector>

#include "critical_chunks.hpp"
#include "ancillary_chunks.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

const byte_t PNG_FILE_SIGNATURE[] = { 137, 80, 78, 71, 13, 10, 26, 10 };

class PNGfile {
    public:
        class Exception : public std::exception {
            public:
                Exception(const char* errorMessage):
                    err{errorMessage} {}
                
                const char* what() const noexcept override {
                    char str[] = "PNG file error: ";
                    return (std::string(str) + err).c_str();
                }

            private:
                std::string err;
        };

        PNGfile(const char* path);

        const unsigned int& width() const
            { return header->width; }

        const unsigned int& height() const
            { return header->height; }

        friend std::ostream& operator<<(std::ostream& out, const PNGfile& obj);

        ~PNGfile() {
            if (header) delete header;
            if (palette) delete palette;
            for (auto& i : ancillaryChunks)
                delete i;
        }

    private:
        IHDR* header = nullptr;
        PLTE* palette = nullptr;

        std::vector<IDAT> imageData;
        std::vector<base_chunk*> criticalChunks;
        std::vector<base_chunk*> ancillaryChunks;

        static bool is_bytes_arr_equal(const byte_t* a, const byte_t* b, const int length = 4) {
            for (int i = 0; i < length; i++)
                if (a[i] != b[i]) return false;
            return true;
        }

        static void read_size_type(std::ifstream &img, unsigned int &size, std::string& type) {
            img.read(reinterpret_cast<char*>(&size), 4);
            MSB_to_LSB(&size, 4);
            byte_t cType[4];
            img.read(reinterpret_cast<char*>(cType), 4);
            type = std::string(reinterpret_cast<char*>(cType), 4);
        }
};