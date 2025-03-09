#include <png.hpp>
#include <msb_to_lsb.hpp>

PNGfile::PNGfile(const char* path) {
    std::ifstream img(path, std::ios::binary);

    if (!img.is_open())
        throw PNGfile::Exception("Couldn't open file.");

    byte_t file_signature[8];
    img.read(reinterpret_cast<char*>(file_signature), 8);

    if (!is_bytes_arr_equal(file_signature, PNG_FILE_SIGNATURE, 8))
        throw PNGfile::Exception("File's signature doesn't match PNG.");

    unsigned int chunkSize;
    img.read(reinterpret_cast<char*>(&chunkSize), 4);
    MSB_to_LSB(&chunkSize, 4);

    byte_t chunkType;
    img.read(reinterpret_cast<char*>(&chunkType), 4);

    if (!is_bytes_arr_equal(&chunkType, reinterpret_cast<const byte_t*>("IHDR")))
        throw PNGfile::Exception("File doesn't contain IHDR chunk");
    
    header = new IHDR(img);
}

std::ostream& operator<<(std::ostream& out, const PNGfile obj) {
    return out << "==============================" << std::endl
               << "           METADATA           " << std::endl
               << "==============================" << std::endl
               << *(obj.header) << std::endl;
}