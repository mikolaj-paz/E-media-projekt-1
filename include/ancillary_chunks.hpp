#pragma once

#include <ostream>
#include <fstream>
#include <string>
#include <map>

#include "chunk.hpp"

#ifndef byte_t
#define byte_t unsigned char
#endif

/// @class tIME
/// @brief Represents the tIME ancillary chunk in a PNG file, which stores the last modification time.
///
/// The tIME chunk contains a timestamp in UTC format, including year, month, day, hour, minute, and second.
///
/// @note It inherits from the `base_chunk` class.
///
class tIME : public base_chunk {
    
    public:
        /// @brief Represents a UTC (Coordinated Universal Time) timestamp.
        ///
        /// This structure is used to store and manipulate a UTC timestamp
        /// with year, month, day, hour, minute, and second components.
        ///
        /// @param byteArray An array of 7 bytes used to initialize the UTC object.
        ///                  The array should contain the encoded values for year,
        ///                  month, day, hour, minute, and second.
        ///
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

        /// @brief Constructor for the tIME chunk class, which represents the tIME ancillary chunk in a PNG file.
        /// @param img Reference to the input file stream from which the chunk data is read.
        /// @param size The size of the tIME chunk data in bytes.
        /// 
        /// This constructor initializes the `base_chunk` with the provided parameters and
        /// extracts the timestamp information from the chunk data.
        ///
        tIME(std::ifstream &img, const unsigned int size):
            base_chunk(img, size, std::string("tIME")),
            timeStamp(data)
        {}

        friend class PNGfile;

        /// @brief Overloads the stream insertion operator to output the contents of a tIME object.
        /// @param out The output stream to which the tIME object will be written.
        /// @param obj The tIME object whose contents will be written to the output stream.
        /// @return A reference to the output stream after writing the tIME object.
        ///
        friend std::ostream& operator<<(std::ostream& out, const tIME& obj);
        
    private:
        UTC timeStamp;
};
    
/// @class tEXt
/// @brief Represents the tEXt ancillary chunk in a PNG file, which stores textual data.
///
/// This class is used to parse and store textual metadata from the tEXt chunk
/// in a PNG file. It includes a keyword that describes the type of text and the actual
/// text content.
///
/// @note This class is derived from the `base_chunk` class.
/// 
class tEXt : public base_chunk {

    public:
        /// @brief Parses a tEXt chunk from the given input stream.
        /// @param img The input file stream containing the image data.
        /// @param size The size of the tEXt chunk in bytes.
        ///
        tEXt(std::ifstream &img, const unsigned int size);
        
        friend class PNGfile;

        /// @brief Overloads the stream insertion operator for the tEXt class.
        /// @param out The output stream to which the tEXt object will be written.
        /// @param obj The tEXt object to be written to the output stream.
        /// @return A reference to the output stream after the tEXt object has been written.
        ///
        friend std::ostream& operator<<(std::ostream& out, const tEXt& obj);
    
    private:
        std::string keyword;
        std::string text;
};

/// @class bKGD
/// @brief Represents the bKGD (Background Color) ancillary chunk in a PNG file.
/// 
/// This class is responsible for handling the background color information
/// stored in the bKGD chunk of a PNG file. It provides functionality to parse
/// and store the background color based on the color type and bit depth of the
/// image. The class supports greyscale, truecolor, and palette-based background
/// color representations.
/// 
/// @note This class is derived from the `base_chunk` class.
/// 
class bKGD : public base_chunk {

    public:
        /// @brief Constructs a bKGD (Background Color) ancillary chunk from the provided image stream.
        ///
        /// This constructor parses the bKGD chunk, which specifies a default background color
        /// to present the image against. The interpretation of the background color depends
        /// on the color type and bit depth of the image.
        ///
        /// @param img The input file stream containing the image data.
        /// @param size The size of the bKGD chunk in bytes.
        /// @param colorType The color type of the image (e.g., grayscale, truecolor, indexed-color).
        /// @param bitDepth The bit depth of the image (e.g., 8-bit, 16-bit).
        ///
        bKGD(std::ifstream &img, const unsigned int size, const byte_t& colorType, const byte_t& bitDepth);

        ~bKGD() {
            if (greyscale) delete greyscale;
            if (truecolor) delete[] truecolor;
            if (paletteIndex) delete paletteIndex;
        }

        friend class PNGfile;

        /// @brief Overloads the stream insertion operator for the bKGD class.
        /// @param out The output stream to which the object will be written.
        /// @param obj The bKGD object to be written to the output stream.
        /// @return A reference to the output stream after writing the object.
        ///
        friend std::ostream& operator<<(std::ostream& out, const bKGD& obj);
    
    private:
        short* greyscale = nullptr;
        short* truecolor = nullptr;
        byte_t* paletteIndex = nullptr;
        
        static const std::map<byte_t, short> bitDepthMask;
};

/// @class gAMA
/// @brief Represents the gAMA ancillary chunk in a PNG file, which stores the image gamma information.
/// 
/// This class is responsible for parsing and storing the gamma value from the gAMA chunk in
/// a PNG file. The gamma value is used to describe the relationship between the image samples
///  and the desired display output intensity.
///
/// @note This class is derived from the `base_chunk` class.
/// 
class gAMA : public base_chunk {

    public:
        /// @brief Constructs the gAMA ancillary chunk in a multimedia file.
        ///
        /// The gAMA chunk specifies the gamma value of the image, which is used
        /// to adjust the brightness and contrast during rendering. This method
        /// reads the gamma value from the provided data stream.
        ///
        /// @param img The input file stream from which the chunk data is read.
        /// @param size The size of the gAMA chunk data in bytes.
        ///
        gAMA(std::ifstream &img, const unsigned int size): base_chunk(img, size, std::string("gAMA"))
            { gamma = read_uint(data); }

        friend class PNGfile;

        /// @brief Overloads the stream insertion operator for the gAMA class.
        /// @param out The output stream to which the gAMA object will be written.
        /// @param obj The gAMA object to be written to the output stream.
        /// @return A reference to the output stream after writing the gAMA object.
        ///
        friend std::ostream& operator<<(std::ostream& out, const gAMA& obj);

    private:
        unsigned int gamma;
};

/// @class pHYs
/// @brief Represents the pHYs ancillary chunk in a PNG file, which specifies the pixel dimensions and unit of measurement.
///
/// @note This class is derived from the `base_chunk` class.
///
class pHYs : public base_chunk {

    public:
        /// @brief Constructs a pHYs ancillary chunk from the given input stream.
        /// @param img The input file stream containing the image data.
        /// @param size The size of the pHYs chunk in bytes.
        ///
        pHYs(std::ifstream &img, const unsigned int size);

        friend class PNGfile;

        /// @brief Overloads the stream insertion operator for the pHYs class.
        /// @param out The output stream to which the object will be written.
        /// @param obj The pHYs object to be written to the output stream.
        /// @return A reference to the output stream after the object has been written.
        ///
        friend std::ostream& operator<<(std::ostream& out, const pHYs& obj);

    private:
        unsigned int pixX;
        unsigned int pixY;
        byte_t unit;
};

/// @class cHRM
/// @brief Represents the cHRM ancillary chunk in a PNG file, which contains chromaticity data.
///
/// This class is used to parse and store chromaticity information from a PNG file's cHRM chunk.
/// It includes white point and primary chromaticity coordinates (red, green, blue).
///
/// @note This class is derived from the `base_chunk` class.
/// 
class cHRM : public base_chunk {

    public:
        /// @brief Constructs a cHRM chunk object by reading data from the provided input stream.
        /// @param img The input file stream to read the chunk data from.
        /// @param size The size of the cHRM chunk in bytes.
        ///
        cHRM(std::ifstream &img, const unsigned int size);

        friend class PNGfile;

        /// @brief Overloads the stream insertion operator for the cHRM class.
        /// @param out The output stream to which the object will be written.
        /// @param obj The cHRM object to be written to the output stream.
        /// @return A reference to the output stream after the object has been written.
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
};