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

        void anonymize();

        void show() const;

        void save(const std::string& path) const;

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

        std::string srcDir;

        void show_with_PLTE() const;

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

        static void save_size_type(std::ofstream& img, unsigned int size, std::string type) {
            LSB_to_MSB(&size, 4);
            img.write(reinterpret_cast<char*>(&size), 4);
            for (auto& c : type)
                img.write(reinterpret_cast<char*>(&c), 1);
        }

        static void save_chunk(std::ofstream& img, const base_chunk* chunk) {
            save_size_type(img, chunk->size, chunk->type);
            img.write(reinterpret_cast<char*>(chunk->data), chunk->size);
            img.write(reinterpret_cast<const char*>(chunk->crc), 4);
        }
};