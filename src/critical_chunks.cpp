#include <critical_chunks.hpp>
#include <helper.hpp>

IHDR::IHDR(std::ifstream &img, const unsigned int size): base_chunk(img, size, std::string("IHDR")) {
    width = read_uint(data);
    height = read_uint(data + 4);

    bitDepth = *(reinterpret_cast<byte_t*>(data + 8));
    colorType = *(reinterpret_cast<byte_t*>(data + 9));

    compressionMethod = *(reinterpret_cast<byte_t*>(data + 10));
    filterMethod = *(reinterpret_cast<byte_t*>(data + 11));
    interlaceMethod = *(reinterpret_cast<byte_t*>(data + 12));
}

PLTE::PLTE(std::ifstream &img, const unsigned int size): base_chunk(img, size, std::string("PLTE")) {
    for (int i = 0; i < size / 3; i++)
        entries.push_back(rgb_entry(*(data + 3*i), *(data + 3*i + 1), *(data + 3*i + 2)));
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
               << "Palette entries: " << obj.entries.size() << std::endl;
}

std::ostream& operator<<(std::ostream& out, const PLTE::rgb_entry& obj) {
    return out << "R:" << obj.red << " G:" << obj.green << " B:" << obj.blue;
}

std::ostream& operator<<(std::ostream& out, const IEND& obj) {
    out << "=== IEND chunk information ===" << std::endl
        << "CRC: ";

    for (auto& i : obj.crc)
        out << static_cast<int>(i) << " ";
    
    return out << std::endl;
}