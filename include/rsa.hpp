#pragma once

#ifndef byte_t
#define byte_t unsigned char
#endif

namespace CryptoPP {
    class Integer;
};

using namespace CryptoPP;

class RSA {
    public:
        RSA() = delete;
        
        static void generate_keys(
            Integer& n,
            Integer& d,
            Integer& e
        );

        static std::pair<std::vector<byte_t>, std::vector<byte_t>> encrypt_half_split(
            const byte_t* data,
            unsigned size,
            const CryptoPP::Integer& n,
            const CryptoPP::Integer& e,
            unsigned short& padding_length,
            const bool& use_ecb = false
        );

        static std::vector<byte_t> decrypt_half_join(
            const byte_t* first_half,
            const byte_t* second_half,
            unsigned cipher_pairs,
            unsigned short padding_length,
            const CryptoPP::Integer& n,
            const CryptoPP::Integer& d,
            const bool& use_ecb = false
        );
};