#include <png.hpp>
#include <rsa.hpp>
#include <SFML/Graphics.hpp>
#include <cryptopp/integer.h>

void PNGfile::encrypt(
    const std::string& path,
    Integer& n,
    Integer& d,
    Integer& e,
    const bool& encrypt_compressed
) {
    RSA::generate_keys(n, d, e);

    if (encrypt_compressed) {
        for (IDAT& idat : imageData)
            encrypt_compressed_idat(idat, n, e);
    }
    else {
        std::vector<byte_t> compressed;
        for (auto& idat : imageData)
            compressed.insert(compressed.end(), idat.data, idat.data + idat.size);

        std::vector<byte_t> filter_bytes;
        auto pixel_data = inflate(compressed, filter_bytes);
        
        unsigned short padding_length = 0;
        auto [first, second] = RSA::encrypt_half_split(
            pixel_data.data(),
            static_cast<unsigned>(pixel_data.size()),
            n,
            e,
            padding_length
        );

        auto recompressed = deflate(first, filter_bytes);

        imageData.clear();
        imageData.push_back(recompressed);

        push_mia_chunk(second, padding_length);
    }
    save(path);
}

void PNGfile::decrypt(
    const std::string& path,
    const CryptoPP::Integer& n,
    const CryptoPP::Integer& d,
    const bool& decrypt_compressed
) {
    if (decrypt_compressed) {
        for (size_t i = 0; i < imageData.size(); ++i) {
            const base_chunk* const eMIA = criticalChunks[criticalChunks.size() - imageData.size() + i];
            decrypt_compressed_idat(imageData[i], eMIA, n, d);
        }
    }
    else {
        base_chunk* mia = nullptr;
        for (auto* chunk : criticalChunks)
            if (chunk->type == "eMIA") { 
                mia = chunk; 
                break;
            }
        
        if (!mia) throw PNGfile::Exception("Missing eMIA chunk.");

        const byte_t* buffer = mia->data;
        unsigned short padding_length = buffer[0] | (buffer[1] << 8);
        const byte_t* second = buffer + 2;
        const unsigned second_size = mia->size - 2;

        std::vector<byte_t> compressed;
        for (auto& idat : imageData)
            compressed.insert(compressed.end(), idat.data, idat.data + idat.size);

        std::vector<byte_t> filter_bytes;
        auto first = inflate(compressed, filter_bytes);

        unsigned k_bytes = n.ByteCount() / 2;
        unsigned cipher_pairs = static_cast<unsigned>(second_size / k_bytes);

        auto decrypted = RSA::decrypt_half_join(
            first.data(),
            second,
            cipher_pairs,
            padding_length,
            n,
            d
        );

        auto recompressed = deflate(decrypted, filter_bytes);

        imageData.clear();
        imageData.push_back(recompressed);
    }
    save(path);
}

std::vector<byte_t> PNGfile::inflate(const std::vector<byte_t>& compressed, std::vector<byte_t>& filter_bytes) {
    auto plain = inflate_buffer(compressed.data(), compressed.size());

    static const std::map<unsigned char, int> color_type_to_channels {
        {0, 1}, {2, 3}, {3, 1}, {4, 2}, {6, 4}
    };
    
    const unsigned stride = 1 + std::ceil(color_type_to_channels.at(header->colorType) * header->bitDepth * width() / 8);
    const unsigned lines = height();

    std::vector<byte_t> pixel_data;
    pixel_data.reserve(plain.size() - lines);
    for (unsigned y = 0; y < lines; ++y) {
        byte_t filter = plain[y * stride];
        filter_bytes.push_back(filter);
        pixel_data.insert(pixel_data.end(), plain.begin() + y * stride + 1, plain.begin() + (y + 1) * stride);
    }

    return std::move(pixel_data);
}

std::vector<byte_t> PNGfile::deflate(const std::vector<byte_t>& pixel_data, const std::vector<byte_t>& filter_bytes) {
    static const std::map<unsigned char, int> color_type_to_channels {
        {0, 1}, {2, 3}, {3, 1}, {4, 2}, {6, 4}
    };
    
    const unsigned stride = 1 + std::ceil(color_type_to_channels.at(header->colorType) * header->bitDepth * width() / 8);
    const unsigned lines = height();

    std::vector<byte_t> encrypted;
    encrypted.reserve(pixel_data.size() + lines);

    auto pixel_data_iterator = pixel_data.begin();
    for (unsigned y = 0; y < lines; ++y) {
        encrypted.push_back(filter_bytes[y]);
        encrypted.insert(encrypted.end(), pixel_data_iterator, pixel_data_iterator + (stride - 1));
        pixel_data_iterator += (stride - 1);
    }
    
    return deflate_buffer(encrypted.data(), encrypted.size());
}

void PNGfile::push_mia_chunk(const std::vector<byte_t>& buffer, const unsigned short& padding_length) {
    std::vector<byte_t> helper;
    helper.reserve(2 + buffer.size());
    helper.push_back(static_cast<byte_t>(padding_length & 0xFF));
    helper.push_back(static_cast<byte_t>(padding_length >> 8));
    helper.insert(helper.end(), buffer.begin(), buffer.end());

    criticalChunks.push_back(new base_chunk(helper, "eMIA"));
}

void PNGfile::encrypt_compressed_idat(IDAT& idat, const Integer& n, const Integer& e) {
    unsigned short padding_length = 0;
    auto [first, second] = RSA::encrypt_half_split(idat.data, idat.size, n, e, padding_length);
    
    replace_idat_data(idat, first);

    push_mia_chunk(second, padding_length);
}

void PNGfile::decrypt_compressed_idat(IDAT& idat, const base_chunk* const sRSA, const Integer& n, const Integer& d) {
    const byte_t* buffer = sRSA->data;
    unsigned short padding = buffer[0] | (buffer[1] << 8);

    const byte_t* second = buffer + 2;
    unsigned cipher_pairs = idat.size / (n.ByteCount() / 2u);

    auto plain = RSA::decrypt_half_join(
        idat.data,
        second,
        cipher_pairs,
        padding,
        n,
        d
    );

    replace_idat_data(idat, plain);
}