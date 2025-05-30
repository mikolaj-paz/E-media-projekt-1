#pragma once

#include <ostream>
#include <fstream>
#include <string>
#include <vector>

#include "chunk.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

/// @class IHDR
/// @brief Represents the IHDR chunk in a PNG file, which contains critical image metadata.
///
/// The IHDR chunk is the first chunk in a PNG file and specifies the image's dimensions,
/// bit depth, color type, compression method, filter method, and interlace method.
///
/// @note This class is a derived class of `base_chunk`.
///
class IHDR : public base_chunk {

    public:
        /// @brief Constructor for the IHDR class, which processes the IHDR chunk of a PNG image.
        /// @param img Input file stream representing the PNG image.
        /// @param size The size of the IHDR chunk in bytes.
        ///
        IHDR(std::ifstream &img, const unsigned int size);

        friend class PNGfile;

        /// Overloads the stream insertion operator to output the contents of an IHDR object
        /// to the provided output stream.
        ///
        /// @param out The output stream to which the IHDR object will be written.
        /// @param obj The IHDR object whose contents will be written to the output stream.
        /// @return A reference to the output stream after the IHDR object has been written.
        ///
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

/// @class IDAT
/// @brief Represents the IDAT chunk in a PNG file, which contains the actual image data.
/// 
/// The IDAT class is used to handle the IDAT chunk in a PNG file. It provides functionality 
/// to read and process the image data stored in this chunk.
/// 
/// @warning This class DOES NOT decompress the data.
///
/// @note This class is a derived class of `base_chunk`.
///
class IDAT : public base_chunk {

    public:
        /// @brief Constructor for the IDAT chunk class.
        ///
        /// Initializes an IDAT chunk by reading data from the provided input stream.
        /// The IDAT chunk is a critical chunk in the PNG file format, used to store
        /// the actual image data, which is typically compressed.
        ///
        /// @param img Reference to the input file stream from which the chunk data is read.
        /// @param size The size of the chunk data in bytes.
        ///
        /// @warning This method DOES NOT decompress the data.
        ///
        IDAT(std::ifstream &img, const unsigned int size): base_chunk(img, size, std::string("IDAT")) {}

        IDAT(const std::vector<byte_t>& buffer): base_chunk(buffer, "IDAT") {}

        friend class PNGfile;

        /// Overloads the stream insertion operator to output the contents of an IDAT object
        /// to the provided output stream.
        ///
        /// @param out The output stream to which the IDAT object will be written.
        /// @param obj The IDAT object to be written to the output stream.
        /// @return A reference to the output stream after the IDAT object has been written.
        ///
        friend std::ostream& operator<<(std::ostream& out, const IDAT& obj);
};

/// @class PLTE
/// @brief Represents the PLTE (Palette) chunk in a PNG file, which contains a palette of colors.
///
/// The PLTE chunk is used in PNG files to define a palette of RGB colors. This class
/// provides functionality to parse and store the palette data.
///
/// @note This class is a derived class of `base_chunk`.
///
class PLTE : public base_chunk {

    public:
        /// @brief Constructor for the PLTE (Palette) chunk.
        /// @param img Reference to the input file stream containing the image data.
        /// @param size The size of the PLTE chunk in bytes.
        ///
        PLTE(std::ifstream &img, const unsigned int size);

        friend class PNGfile;

        /// Overloads the stream insertion operator for the PLTE class.
        /// Outputs only the number of entries in chunk.
        ///
        /// @param out The output stream to which the content will be written.
        /// @param obj The PLTE object to be written to the output stream.
        /// @return A reference to the output stream after the PLTE object content has been written.
        ///
        friend std::ostream& operator<<(std::ostream& out, const PLTE& obj);

    private:
        /// @brief Represents an RGB color entry with red, green, and blue components.
        ///
        /// This structure is used to store individual color components as bytes.
        /// Each component (red, green, blue) is represented by a `byte_t`.
        ///
        struct rgb_entry {
            rgb_entry(const byte_t& red, const byte_t& green, const byte_t& blue):
                red{red}, green{green}, blue{blue} {}

            byte_t red;
            byte_t green;
            byte_t blue;
        };

        std::vector<rgb_entry> entries;
    
    public:
        /// Overloads the stream insertion operator to output the contents of an 
        /// `rgb_entry` object to the provided output stream.
        /// 
        /// @param out The output stream to which the object will be written.
        /// @param obj The `rgb_entry` object to be written to the output stream.
        /// @return A reference to the output stream after the object has been written.
        friend std::ostream& operator<<(std::ostream& out, const rgb_entry& obj);
};

/// @class IEND
/// @brief Represents the IEND chunk in a PNG file, which marks the end of the PNG data stream.
/// 
/// The IEND chunk is a critical chunk in the PNG file format. It does not contain any data
/// and serves as a marker to indicate the end of the PNG file.
/// 
/// @note This class inherits from the `base_chunk` class.
///
class IEND : public base_chunk {

    public:
        /// @brief Represents the IEND chunk in a PNG file format. The IEND chunk marks
        ///        the end of the image data.
        /// @param img Reference to the input file stream from which the chunk data is read.
        ///
        IEND(std::ifstream &img): base_chunk(img, 0u, std::string("IEND")) {}

        friend class PNGfile;

        /// Overloads the stream insertion operator to output the contents of an IEND object
        /// to the provided output stream.
        ///
        /// @param out The output stream to which the IEND object will be written.
        /// @param obj The IEND object to be written to the output stream.
        /// @return A reference to the output stream after the IEND object has been written.
        ///
        friend std::ostream& operator<<(std::ostream& out, const IEND& obj);
};