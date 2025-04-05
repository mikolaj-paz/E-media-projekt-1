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

pHYs::pHYs(std::ifstream &img, const unsigned int size, const std::string type): base_chunk(img, size, type) {
    pixX = read_uint(data);
    pixY = read_uint(data + 4);
    unit = *(reinterpret_cast<unsigned int*>(data + 8));
}

cHRM::cHRM(std::ifstream &img, const unsigned int size, const std::string type): base_chunk(img, size, type) {
    wpx = read_uint(data);
    wpy = read_uint(data + 4);
    rx = read_uint(data + 8);
    ry = read_uint(data + 12);
    gx = read_uint(data + 16);
    gy = read_uint(data + 20);
    bx = read_uint(data + 24);
    by = read_uint(data + 28);
}

std::ostream& operator<<(std::ostream& out, const tIME::UTC& obj) {
    out << obj.year << '-';
    if (obj.month < 10) out << '0';
    out << obj.month << '-';
    if (obj.day < 10) out << '0';
    out << obj.day << 'T';
    if (obj.hour < 10) out << '0';
    out << obj.hour << ':';
    if (obj.minute < 10) out << '0';
    out << obj.minute << ':';
    if (obj.second < 10) out << '0';
    out << obj.second << 'U';

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
        return out << "Greyscale: " << *(obj.greyscale) << std::endl;
    else if (obj.truecolor)
        return out << "Red: " << obj.truecolor[0] << std::endl
                   << "Green: " << obj.truecolor[1] << std::endl
                   << "Blue: " << obj.truecolor[2] << std::endl;
    else
        return out << "Palette index: " << *(obj.paletteIndex) << std::endl;
}

std::ostream& operator<<(std::ostream& out, const gAMA& obj) {
    return out << "=== gAMA chunk information ===" << std::endl
               << "Image gamma: " << static_cast<float>(obj.gamma) / 1E5f << std::endl;
}

std::ostream& operator<<(std::ostream& out, const pHYs& obj) {
    return out << "=== gAMA chunk information ===" << std::endl
               << "Pixels per unit, X axis: " << obj.pixX << std::endl
               << "Pixels per unit, Y axis: " << obj.pixY << std::endl
               << "Unit: " << static_cast<int>(obj.unit) << std::endl;
}

std::ostream& operator<<(std::ostream& out, const cHRM& obj) {
    return out << "=== cHRM chunk information ===" << std::endl
               << "White Point x: " << static_cast<float>(obj.wpx) / 1E5f << std::endl
               << "White Point y: " << static_cast<float>(obj.wpy) / 1E5f << std::endl
               << "Red x: " << static_cast<float>(obj.rx) / 1E5f << std::endl
               << "Red y: " << static_cast<float>(obj.ry) / 1E5f << std::endl
               << "Green x: " << static_cast<float>(obj.gx) / 1E5f << std::endl
               << "Green y: " << static_cast<float>(obj.gy) / 1E5f << std::endl
               << "Blue x: " << static_cast<float>(obj.bx) / 1E5f << std::endl
               << "Blue y: " << static_cast<float>(obj.by) / 1E5f << std::endl;
}