#include <critical_chunks.hpp>
#include <msb_to_lsb.hpp>

IHDR::IHDR(std::ifstream &img, const unsigned int size, const byte_t type[4]): base_chunk(img, size, type) {
    width = *(reinterpret_cast<unsigned int*>(data));
    MSB_to_LSB(&width, 4);
    height = *(reinterpret_cast<unsigned int*>(data + 4));
    MSB_to_LSB(&height, 4);

    bit_depth = *(reinterpret_cast<unsigned char*>(data + 8));
    color_type = *(reinterpret_cast<unsigned char*>(data + 9));

    compression_method = *(reinterpret_cast<unsigned char*>(data + 10));
    filter_method = *(reinterpret_cast<unsigned char*>(data + 11));
    interlace_method = *(reinterpret_cast<unsigned char*>(data + 12));
}

std::ostream& operator<<(std::ostream& out, const IHDR& obj) {
    return out << "=== IHDR chunk information ===" << std::endl
               << "Width:              " << obj.width << std::endl
               << "Height:             " << obj.height << std::endl
               << "Bit depth:          " << static_cast<int>(obj.bit_depth) << std::endl
               << "Color type:         " << static_cast<int>(obj.color_type) << std::endl
               << "Compression method: " << static_cast<int>(obj.compression_method) << std::endl
               << "Filter method:      " << static_cast<int>(obj.filter_method) << std::endl
               << "Interlace method:   " << static_cast<int>(obj.interlace_method) << std::endl;
}

std::ostream& operator<<(std::ostream& out, const IDAT& obj) {
    return out << "=== IDAT chunk information ===" << std::endl
               << "Data size: " << obj.size << std::endl;
}