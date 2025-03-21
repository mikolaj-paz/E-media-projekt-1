#include <png.hpp>
#include <msb_to_lsb.hpp>

#include <iostream>

PNGfile::PNGfile(const char* path) {
    std::ifstream img(path, std::ios::binary);

    if (!img.is_open())
        throw PNGfile::Exception("Couldn't open file.");

    byte_t file_signature[8];
    img.read(reinterpret_cast<char*>(file_signature), 8);

    if (!is_bytes_arr_equal(file_signature, PNG_FILE_SIGNATURE, 8))
        throw PNGfile::Exception("File's signature doesn't match PNG.");

    unsigned int chunkSize;
    std::string chunkType;
    read_size_type(img, chunkSize, chunkType);

    if (chunkType != "IHDR")
        throw PNGfile::Exception("File doesn't contain IHDR chunk");
    
    header = new IHDR(img, chunkSize, chunkType);

    // switch (header->colorType) {
    //     case 3:
    //         if (chunkType != "PLTE")
    //             throw PNGfile::Exception("Mandatory PLTE chunk not found.");
    //         palette = new PLTE(img, chunkSize, chunkType);
    //         break;
    //     case 2:
    //     case 6:
    //         if (chunkType == "PLTE")
    //             palette = new PLTE(img, chunkSize, chunkType);
    //         break;
    //     case 0:
    //     case 4:
    //         if (chunkType == "PLTE")
    //             throw PNGfile::Exception("PLTE chunk should not be present");
    //         break;

    //     default:
    //         throw PNGfile::Exception("Invalid color type.");

    //     img.close();
    // }
    
    int temp = 0;
    while (chunkType != "IEND") {
        read_size_type(img, chunkSize, chunkType);

        if (chunkType == "IDAT")
            imageData.push_back(IDAT(img, chunkSize, chunkType));
        else if (chunkType == "PLTE")
            criticalChunks.push_back(new PLTE(img, chunkSize, chunkType));
        else if (chunkType == "IEND")
            criticalChunks.push_back(new IEND(img, chunkSize, chunkType));
        else if (chunkType == "tIME")
            ancillaryChunks.push_back(new tIME(img, chunkSize, chunkType));
        else if (chunkType == "tEXt")
            ancillaryChunks.push_back(new tEXt(img, chunkSize, chunkType));
        else if (chunkType == "bKGD")
            ancillaryChunks.push_back(new bKGD(img, chunkSize, chunkType, header->colorType, header->bitDepth));
        else if (chunkType == "gAMA")
            ancillaryChunks.push_back(new gAMA(img, chunkSize, chunkType));
        else if (chunkType == "pHYs")
            ancillaryChunks.push_back(new pHYs(img, chunkSize, chunkType));
        else if (chunkType == "cHRM")
            ancillaryChunks.push_back(new cHRM(img, chunkSize, chunkType));
        else
            ancillaryChunks.push_back(new base_chunk(img, chunkSize, chunkType));
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
    
    for (auto& i : obj.criticalChunks) {
        if (i->type == "PLTE")
            out << *(static_cast<PLTE*>(i));
        else if (i->type == "IEND")
            continue;
        else
            throw PNGfile::Exception("Unsupported critical chunk found.");
    }

    for (auto& i : obj.ancillaryChunks) {
        if (i->type == "tIME")
            out << *(static_cast<tIME*>(i));
        else if (i->type == "tEXt")
            out << *(static_cast<tEXt*>(i));
        else if (i->type == "bKGD")
            out << *(static_cast<bKGD*>(i));
        else if (i->type == "gAMA")
            out << *(static_cast<gAMA*>(i));
        else if (i->type == "pHYs")
            out << *(static_cast<pHYs*>(i));
        else if (i->type == "cHRM")
            out << *(static_cast<cHRM*>(i));
        else
            out << *i;
    }

    out << *(static_cast<IEND*>(obj.criticalChunks.back()));
    
    return out;
}