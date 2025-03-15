#include <critical_chunks.hpp>
#include <msb_to_lsb.hpp>

IHDR::IHDR(std::ifstream &img, const unsigned int size, const std::string& type): base_chunk(img, size, type) {
    width = *(reinterpret_cast<unsigned int*>(data));
    MSB_to_LSB(&width, 4);
    height = *(reinterpret_cast<unsigned int*>(data + 4));
    MSB_to_LSB(&height, 4);

    bitDepth = *(reinterpret_cast<unsigned char*>(data + 8));
    colorType = *(reinterpret_cast<unsigned char*>(data + 9));

    compressionMethod = *(reinterpret_cast<unsigned char*>(data + 10));
    filterMethod = *(reinterpret_cast<unsigned char*>(data + 11));
    interlaceMethod = *(reinterpret_cast<unsigned char*>(data + 12));
}

std::ostream& operator<<(std::ostream& out, const IHDR& obj) {
    return out << "=== IHDR chunk information ===" << std::endl
               << "Width:              " << obj.width << std::endl
               << "Height:             " << obj.height << std::endl
               << "Bit depth:          " << static_cast<int>(obj.bitDepth) << std::endl
               << "Color type:         " << static_cast<int>(obj.colorType) << std::endl
               << "Compression method: " << static_cast<int>(obj.compressionMethod) << std::endl
               << "Filter method:      " << static_cast<int>(obj.filterMethod) << std::endl
               << "Interlace method:   " << static_cast<int>(obj.interlaceMethod) << std::endl;
}

std::ostream& operator<<(std::ostream& out, const IDAT& obj) {
    out << "=== IDAT chunk information ===" << std::endl
        << "Data size: " << obj.size << std::endl
        << "CRC: ";

    for (auto& i : obj.crc)
        out << static_cast<int>(i) << " ";
    
    return out << std::endl;
}

std::ostream& operator<<(std::ostream& out, const PLTE& obj) {
    return out << "=== PLTE chunk information ===" << std::endl
               << "Data size: " << obj.size << std::endl;
}