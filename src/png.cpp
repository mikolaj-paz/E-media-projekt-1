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
    byte_t chunkType[4];
    read_size_type(img, chunkSize, chunkType);

    if (!is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("IHDR")))
        throw PNGfile::Exception("File doesn't contain IHDR chunk");
    
    header = new IHDR(img, chunkSize, chunkType);

    switch (header->color_type) {
        case 3:
            if (!is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("PLTE")))
                throw PNGfile::Exception("Mandatory PLTE chunk not found.");
            palette = new PLTE(img, chunkSize, chunkType);
            break;
        case 2:
        case 6:
            if (is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("PLTE")))
                palette = new PLTE(img, chunkSize, chunkType);
            break;
        case 0:
        case 4:
            if (is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("PLTE")))
                throw PNGfile::Exception("PLTE chunk should not be present");
            break;

        default:
            throw PNGfile::Exception("Invalid color type.");

        img.close();
    }
    
    while (!is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("IEND"))) {
        read_size_type(img, chunkSize, chunkType);

        if (is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("IDAT"))) {
            imageData.push_back(IDAT(img, chunkSize, chunkType));
        }
        else if (is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("tIME"))) {
            ancillaryChunks.push_back(new tIME(img, chunkSize, chunkType));
        }
        else if (is_bytes_arr_equal(chunkType, reinterpret_cast<const byte_t*>("tEXt"))) {
            ancillaryChunks.push_back(new tEXt(img, chunkSize, chunkType));
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

    if (obj.palette)
        out << *(obj.palette);
    
    for (auto& i : obj.imageData)
        out << i;
    
    for (auto& i : obj.ancillaryChunks) {
        if (PNGfile::is_bytes_arr_equal(i->type, reinterpret_cast<const byte_t*>("tIME")))
            out << *(static_cast<tIME*>(i));
        else if (PNGfile::is_bytes_arr_equal(i->type, reinterpret_cast<const byte_t*>("tEXt")))
            out << *(static_cast<tEXt*>(i));
        else
            out << *i;
    }
        
    
    return out;
}