#pragma once

#include <exception>
#include <cstring>
#include <fstream>

#include "critical_chunks.hpp"

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
                    return strcat(str, err);
                }

            private:
                const char* err;
        };

        PNGfile(const char* path);

        friend std::ostream& operator<<(std::ostream& out, const PNGfile obj);

        ~PNGfile()
            { delete header; }

    private:
        IHDR* header;

        static bool is_bytes_arr_equal(const byte_t* a, const byte_t* b, const int length = 4) {
            for (int i = 0; i < length; i++)
                if (a[i] != b[i]) return false;
            return true;
        }
};