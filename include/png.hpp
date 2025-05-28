#pragma once

#include <exception>
#include <cstring>
#include <fstream>
#include <vector>

#include "critical_chunks.hpp"
#include "ancillary_chunks.hpp"
#include "rsa.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

const byte_t PNG_FILE_SIGNATURE[] = { 137, 80, 78, 71, 13, 10, 26, 10 };

/// @class PNGfile
/// @brief Represents a PNG file, providing functionality to manipulate and save PNG images.
/// 
/// This class handles reading, modifying, and saving PNG files. It includes support for
/// managing PNG headers, palettes, image data, and ancillary chunks.
///
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

        /// @brief Constructor for the PNGfile class. Parses a PNG file and initializes its components.
        /// @param path The file path to the PNG file to be loaded.
        /// @throws `PNGfile::Exception` if the file cannot be opened, the file signature does not match PNG,
        ///         the file does not contain an IHDR chunk, or if there are issues with mandatory chunks
        ///         based on the color type.
        /// 
        /// This constructor performs the following steps:
        /// - Opens the file in binary mode.
        /// - Verifies the PNG file signature.
        /// - Reads the first chunk (`IHDR`) and initializes the header.
        /// - Iterates through the file to read and process all chunks:
        ///   - Critical chunks such as IDAT, PLTE, and IEND are handled specifically.
        ///   - Ancillary chunks such as tIME, tEXt, bKGD, gAMA, pHYs, and cHRM are processed and stored.
        ///   - Unknown chunks are stored as `base_chunk` objects.
        /// - Validates the presence or absence of the PLTE chunk based on the color type.
        ///
        PNGfile(const char* path);

        /// Retrieves the width of the PNG image.
        /// @return A constant reference to an unsigned integer representing the width of the image.
        const unsigned int& width() const
            { return header->width; }

        /// Retrieves the height of the PNG image.
        /// @return A constant reference to an unsigned integer representing the height of the image.
        const unsigned int& height() const
            { return header->height; }

        void modify(const std::string& path);

        void anonymize(const std::string& src, const std::string& dst);

        void encrypt(const std::string& path, const bool& encrypt_compressed = true);

        /// @brief Displays the PNG image using SFML.
        void show() const;

        /// Saves the image to a file at the specified path.
        /// 
        /// @param path The file path where the object will be saved.
        void save(const std::string& path) const;

        /// Overloads the stream insertion operator to allow outputting the contents
        /// of a PNGfile object to an output stream.
        /// @param out The output stream to which the PNGfile object will be written.
        /// @param obj The PNGfile object to be outputted.
        /// @return A reference to the output stream after writing the PNGfile object.
        ///
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

        /// @brief Displays the PNG image with the PLTE (palette) chunk information.
        ///
        /// @note Ensure that the PNG file contains a valid PLTE chunk before calling this function.
        ///
        void show_with_PLTE() const;

        /// @brief Compares two byte arrays for equality.
        /// @param a Pointer to the first byte array.
        /// @param b Pointer to the second byte array.
        /// @param length The number of bytes to compare (default is 4).
        /// @return True if the arrays are equal for the specified length, otherwise false.
        ///
        static bool is_bytes_arr_equal(const byte_t* a, const byte_t* b, const int length = 4) {
            for (int i = 0; i < length; i++)
                if (a[i] != b[i]) return false;
            return true;
        }

        /// @brief Reads the size and type of a PNG chunk from the given input stream.
        /// 
        /// This function reads 8 bytes from the input stream: the first 4 bytes represent
        /// the size of the chunk and the next 4 bytes represent the type of the chunk as a string.
        ///
        /// @param img The input file stream to read from.
        /// @param size A reference to an unsigned integer where the chunk size will be stored.
        /// @param type A reference to a string where the chunk type will be stored.
        ///
        static void read_size_type(std::ifstream &img, unsigned int &size, std::string& type) {
            img.read(reinterpret_cast<char*>(&size), 4);
            MSB_to_LSB(&size, 4);
            byte_t cType[4];
            img.read(reinterpret_cast<char*>(cType), 4);
            type = std::string(reinterpret_cast<char*>(cType), 4);
        }

        /// @brief Saves the size and type of a PNG chunk to an output file stream.
        /// @param img The output file stream where the size and type will be written.
        /// @param size The size of the PNG chunk in bytes.
        /// @param type The type of the PNG chunk as a string.
        ///
        static void save_size_type(std::ofstream& img, unsigned int size, std::string type) {
            LSB_to_MSB(&size, 4);
            img.write(reinterpret_cast<char*>(&size), 4);
            for (auto& c : type)
                img.write(reinterpret_cast<char*>(&c), 1);
        }

        /// @brief Saves a PNG chunk to an output file stream.
        /// @param img The output file stream where the chunk will be written.
        /// @param chunk A pointer to the chunk to be saved. The chunk contains the size, type, data, and CRC.
        /// 
        /// This function writes the chunk's size and type, followed by its data and CRC, to the specified output file stream.
        ///
        static void save_chunk(std::ofstream& img, const base_chunk* chunk) {
            save_size_type(img, chunk->size, chunk->type);
            img.write(reinterpret_cast<char*>(chunk->data), chunk->size);
            img.write(reinterpret_cast<const char*>(chunk->crc), 4);
        }
};