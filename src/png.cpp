#include <png.hpp>
#include <helper.hpp>

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <algorithm>

PNGfile::PNGfile(const char* path) {
    std::ifstream img(path, std::ios::binary);

    if (!img.is_open())
        throw PNGfile::Exception("Couldn't open file.");
    
    srcDir = path;

    byte_t file_signature[8];
    img.read(reinterpret_cast<char*>(file_signature), 8);

    if (!is_bytes_arr_equal(file_signature, PNG_FILE_SIGNATURE, 8))
        throw PNGfile::Exception("File's signature doesn't match PNG.");

    unsigned int chunkSize;
    std::string chunkType;
    read_size_type(img, chunkSize, chunkType);

    if (chunkType != "IHDR")
        throw PNGfile::Exception("File doesn't contain IHDR chunk");
    
    header = new IHDR(img, chunkSize);
    
    int temp = 0;
    while (img.peek() != EOF) {
        read_size_type(img, chunkSize, chunkType);

        if (chunkType == "IDAT")
            imageData.push_back(IDAT(img, chunkSize));
        else if (chunkType == "PLTE")
            palette = new PLTE(img, chunkSize);
        else if (chunkType == "IEND")
            criticalChunks.push_back(new IEND(img));
        else if (chunkType == "tIME")
            ancillaryChunks.push_back(new tIME(img, chunkSize));
        else if (chunkType == "tEXt")
            ancillaryChunks.push_back(new tEXt(img, chunkSize));
        else if (chunkType == "bKGD")
            ancillaryChunks.push_back(new bKGD(img, chunkSize, header->colorType, header->bitDepth));
        else if (chunkType == "gAMA")
            ancillaryChunks.push_back(new gAMA(img, chunkSize));
        else if (chunkType == "pHYs")
            ancillaryChunks.push_back(new pHYs(img, chunkSize));
        else if (chunkType == "cHRM")
            ancillaryChunks.push_back(new cHRM(img, chunkSize));
        else {
            if (chunkType[0] > 'A' && chunkType[0] < 'Z')
                criticalChunks.push_back(new base_chunk(img, chunkSize, chunkType));
            else
                ancillaryChunks.push_back(new base_chunk(img, chunkSize, chunkType));
        }
    }

    switch (header->colorType) {
        case 3:
            if (!palette)
                throw PNGfile::Exception("Mandatory PLTE chunk not found.");
            break;
        case 2:
        case 6:
            break;
        case 0:
        case 4:
            if (palette)
                throw PNGfile::Exception("PLTE chunk should not be present");
            break;

        default:
            throw PNGfile::Exception("Invalid color type.");
    }

    img.close();
}

void PNGfile::modify(const std::string& path) {
    for (auto& dat : imageData) {
        dat.crc[0] = 1;
        dat.crc[1] = 1;
        dat.crc[2] = 1;
        dat.crc[3] = 1;
    }

    std::ifstream blank;
    criticalChunks.push_back(new base_chunk(blank, 0, "sECR"));

    save(path);
}

void PNGfile::show() const {
    if (palette) {
        show_with_PLTE();
        return;
    }

    sf::RenderWindow window(sf::VideoMode({width(), height()}), "Image");
    const sf::Texture tex(srcDir);
    const sf::Sprite sprite(tex);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }    
}

void PNGfile::show_with_PLTE() const {
    unsigned int entrySize = 20u;
    unsigned int windowWidth;

    unsigned int numInRow = palette->entries.size() > 32u ? 32u : palette->entries.size();

    if (width() > numInRow * entrySize) {
        windowWidth = width();
        entrySize = width() / numInRow;
    }
    else windowWidth = numInRow * entrySize;

    unsigned int windowHeight = height() + entrySize * std::ceil(palette->entries.size() / static_cast<double>(numInRow));

    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Image");
    const sf::Texture tex(srcDir);
    const sf::Sprite sprite(tex);

    unsigned int i = 0u;
    sf::RectangleShape entries[palette->entries.size()];

    int k = 0;
    for (unsigned int j = 0; j < std::ceil(palette->entries.size() / static_cast<double>(numInRow)); ++j)
        for (unsigned int i = 0; i < numInRow; ++i) {
            entries[k].setSize({static_cast<float>(entrySize), static_cast<float>(entrySize)});
            entries[k].setFillColor(sf::Color(palette->entries[k].red, palette->entries[k].green, palette->entries[k].blue));
            entries[k].setPosition({static_cast<float>(entrySize * i), static_cast<float>(height() + entrySize * j)});
            ++k;
        }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(sprite);
        for (auto& entry : entries)
            window.draw(entry);
        window.display();
    }
}

void PNGfile::save(const std::string& path) const {
    std::ofstream img(path, std::ios::binary);

    img.write(reinterpret_cast<const char*>(PNG_FILE_SIGNATURE), 8);

    save_chunk(img, header);

    if (palette) save_chunk(img, palette);

    for (auto i : imageData)
        save_chunk(img, &i);

    for (auto i : ancillaryChunks)
        save_chunk(img, i);
    
    for (auto i : criticalChunks)
        save_chunk(img, i);

    img.close();
}

void PNGfile::replace_idat_data(IDAT& idat, const std::vector<byte_t>& src) {
    byte_t* new_buf = new byte_t[src.size()];
    std::memcpy(new_buf, src.data(), src.size());

    delete[] idat.data;

    idat.data = new_buf;
    idat.size = static_cast<unsigned>(src.size());
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
        if (i->type == "IEND")
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