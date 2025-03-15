#include <ancillary_chunks.hpp>

#include <bitset>

tIME::UTC::UTC(const byte_t arr[7]) {
    year = static_cast<int>((arr[0] << 8) | arr[1]);
    month = static_cast<int>(arr[2]);
    day = static_cast<int>(arr[3]);
    hour = static_cast<int>(arr[4]);
    minute = static_cast<int>(arr[5]);
    second = static_cast<int>(arr[6]);
}

tEXt::tEXt(std::ifstream &img, const unsigned int size, const std::string type): base_chunk(img, size, type) {
    byte_t inputByte = 0;
    int bytesRead = 0;

    for (int i = 0; i < 79; i++) {
        inputByte = *(data + bytesRead++);
        if (inputByte == 0) break;
        keyword += inputByte;
    }

    do
        text += *(data + bytesRead++);
    while (bytesRead < size);
}

const std::map<byte_t, short> bKGD::bitDepthMask {
    { 1, 0x1 },
    { 2, 0x3 },
    { 4, 0xF },
    { 8, 0xFF },
    { 16, 0xFFFF }
};

bKGD::bKGD(std::ifstream &img, const unsigned int size, const std::string type, const byte_t& colorType, const byte_t& bitDepth): 
base_chunk(img, size, type) {
    switch (colorType) {
        case 0:
        case 4:
            greyscale = new short(((data[0] << 8) | data[1]) ^ bitDepthMask.at(bitDepth));
            break;
        case 2:
        case 6:
            truecolor = new short[3];
            truecolor[0] = ((data[0] << 8) | data[1]) ^ bitDepthMask.at(bitDepth);
            truecolor[1] = ((data[2] << 8) | data[3]) ^ bitDepthMask.at(bitDepth);
            truecolor[2] = ((data[4] << 8) | data[5]) ^ bitDepthMask.at(bitDepth);
            break;
        case 3:
            paletteIndex = new byte_t(data[0]);
            break;
    }
}

std::ostream& operator<<(std::ostream& out, const tIME::UTC& obj) {
    out << obj.year << '-';
    if (obj.month < 10) out << '0';
    out << obj.month << '-';
    if (obj.day < 10) out << '0';
    out << obj.day << ' ';
    if (obj.hour < 10) out << '0';
    out << obj.hour << ':';
    if (obj.minute < 10) out << '0';
    out << obj.minute << ':';
    if (obj.second < 10) out << '0';
    out << obj.second << ' ';

    return out;
}

std::ostream& operator<<(std::ostream& out, const tIME& obj) {
    return out << "=== tIME chunk information ===" << std::endl
               << "Modified: " << obj.timeStamp << std::endl;
}

std::ostream& operator<<(std::ostream& out, const tEXt& obj) {
    return out << "=== tEXt chunk information ===" << std::endl
               << "Keyword: " << obj.keyword << std::endl
               << "Text: " << obj.text << std::endl;
}

std::ostream& operator<<(std::ostream& out, const bKGD& obj) {
    out << "=== bKGD chunk information ===" << std::endl
        << "------ Background color ------" << std::endl;

    if (obj.greyscale)
        return out << "Greyscale: " << obj.greyscale << std::endl;
    else if (obj.truecolor)
        return out << "Red: " << obj.truecolor[0] << std::endl
                   << "Green: " << obj.truecolor[1] << std::endl
                   << "Blue: " << obj.truecolor[2] << std::endl;
    else
        return out << "Palette index: " << obj.paletteIndex << std::endl;
}