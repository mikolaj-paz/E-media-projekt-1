#include <png.hpp>
#include <cryptopp/zlib.h>

using namespace CryptoPP;

std::vector<byte_t> PNGfile::inflate_buffer(const byte_t* data, const unsigned& size) {
    std::vector<byte_t> out;
    try {
        ArraySource src(data, size, true, 
            new ZlibDecompressor(
                new VectorSink(out)
            )
        );
    }
    catch (const CryptoPP::Exception& e) {
        throw PNGfile::Exception("Inflate (CryptoPP) failed.");
    }
    return std::move(out);
}

std::vector<byte_t> PNGfile::deflate_buffer(const byte_t* data, const unsigned& size, const int& compression_level) {
    std::vector<byte_t> out;
    try {
        ArraySource src(data, size, true,
            new ZlibCompressor(
                new VectorSink(out),
                compression_level
            )
        );
    }
    catch (const CryptoPP::Exception& e) {
        throw PNGfile::Exception("Deflate (CryptoPP) failed.");
    }
    return std::move(out);
}