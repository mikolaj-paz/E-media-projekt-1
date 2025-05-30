#pragma once

#include <fstream>
#include <vector>
#include <cstring>

#include "helper.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

class PNGfile;

/// @class base_chunk
/// @brief Represents a base class for handling PNG file chunks.
class base_chunk {
    public:
        /// @brief Copy constructor for the base_chunk class.
        /// @param other The base_chunk object to copy from.
        /// 
        /// This constructor performs a deep copy of the `other` base_chunk object. 
        /// It copies the size and type directly, and performs element-wise copying 
        /// of the `crc` array. Additionally, it allocates new memory for the `data` 
        /// array and copies its contents from the `other` object.
        ///
        base_chunk(const base_chunk& other): size{other.size}, type{other.type} {
            for (int i = 0; i < 4; i++) {
                crc[i] = other.crc[i];
            }
            data = new byte_t[size];
            for (int i = 0; i < size; i++)
                data[i] = other.data[i];           
        }

        /// @brief Copy assignment operator for the base_chunk class.
        /// @param other The base_chunk object to be copied.
        /// @return A reference to the current base_chunk object after assignment.
        ///
        base_chunk& operator=(const base_chunk& other)
            { return *this = base_chunk(other); }

        /// @brief Constructs a base_chunk object by reading chunk data and CRC from an input stream.
        /// @param img Reference to the input file stream to read the chunk data and CRC from.
        /// @param size The size of the chunk data in bytes.
        /// @param type The type of the chunk as a string.
        ///
        base_chunk(std::ifstream &img, const unsigned int size, const std::string type): size{size}, type{type} {
            // Chunk data
            data = new byte_t[size];
            img.read(reinterpret_cast<char*>(data), size);

            // CRC
            img.read(reinterpret_cast<char*>(crc), 4);
        }

        base_chunk(const std::vector<byte_t>& buffer, const std::string type): 
            size{static_cast<unsigned>(buffer.size())}, type{type} 
        {
            data = new byte_t[size];
            std::memcpy(data, buffer.data(), size);

            crc[0] = 27;
            crc[1] = 2;
            crc[2] = 57;
            crc[3] = 1;
        }

        ~base_chunk()
            { delete[] data; }

        /// @brief Overloads the stream insertion operator for the base_chunk class.
        ///
        /// Calling this function means that chunk type is not supported.
        ///
        /// @param out The output stream to write to.
        /// @param obj The base_chunk object to be represented as a string.
        /// @return A reference to the output stream with the formatted chunk information.
        ///
        friend std::ostream& operator<<(std::ostream& out, const base_chunk& obj) {
            return out << "=== " << obj.type << " chunk information ===" << std::endl
                       << "not supported" << std::endl;
        }

        friend class PNGfile;
        friend std::ostream& operator<<(std::ostream& out, const PNGfile& obj);

    protected:
        unsigned int size;
        std::string type;
        byte_t* data;
        byte_t crc[4];
};