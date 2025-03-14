#include <png.hpp>
#include <msb_to_lsb.hpp>

PNGfile::PNGfile(const char* path): ancillaryChunks{0} {
    std::ifstream img(path, std::ios::binary);

    if (!img.is_open())
        throw PNGfile::Exception("Couldn't open file.");

    byte_t file_signature[8];
    img.read(reinterpret_cast<char*>(file_signature), 8);

    if (!is_bytes_arr_equal(file_signature, PNG_FILE_SIGNATURE, 8))
        throw PNGfile::Exception("File's signature doesn't match PNG.");

    unsigned int chunkSize;
    byte_t chunkType[4];
    read_size_type(img, chunkSize, chunkType);

    if (!is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("IHDR")))
        throw PNGfile::Exception("File doesn't contain IHDR chunk");
    
    header = new IHDR(img, chunkSize, chunkType);
    
    while (!is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("IEND"))) {
        read_size_type(img, chunkSize, chunkType);

        if (is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("IDAT"))) {
            imageData.push_back(IDAT(img, chunkSize, chunkType));
        }
        else {
            ancillaryChunks.push_back(new base_chunk(img, chunkSize, chunkType));
        }
    }
}

std::ostream& operator<<(std::ostream& out, const PNGfile& obj) {
    out << "==============================" << std::endl
        << "           METADATA           " << std::endl
        << "==============================" << std::endl
        << *(obj.header);
    
    for (auto& i : obj.imageData)
        out << i;
    
    for (auto& i : obj.ancillaryChunks)
        if (i) out << *i;
    
    return out;
}