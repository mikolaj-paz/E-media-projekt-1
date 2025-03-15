#include <ancillary_chunks.hpp>

#include <bitset>

tIME::UTC::UTC(const byte_t arr[7]) {
    year = static_cast<int>((arr[0] << 8) ^ arr[1]);
    month = static_cast<int>(arr[2]);
    day = static_cast<int>(arr[3]);
    hour = static_cast<int>(arr[4]);
    minute = static_cast<int>(arr[5]);
    second = static_cast<int>(arr[6]);
}

tEXt::tEXt(std::ifstream &img, const unsigned int size, const byte_t type[4]): base_chunk(img, size, type) {
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
    out << obj.second << 'Z';

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