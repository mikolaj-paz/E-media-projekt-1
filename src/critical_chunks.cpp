#include <critical_chunks.hpp>
#include <msb_to_lsb.hpp>

void IHDR::read(std::ifstream &img)
{
    img.read(reinterpret_cast<char*>(&width), 4);
    MSB_to_LSB(&width, 4);
    img.read(reinterpret_cast<char*>(&height), 4);
    MSB_to_LSB(&height, 4);

    img.read(reinterpret_cast<char*>(&bit_depth), 1);
    img.read(reinterpret_cast<char*>(&color_type), 1);

    img.read(reinterpret_cast<char*>(&compression_method), 1);
    img.read(reinterpret_cast<char*>(&filter_method), 1);
    img.read(reinterpret_cast<char*>(&interlace_method), 1);
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