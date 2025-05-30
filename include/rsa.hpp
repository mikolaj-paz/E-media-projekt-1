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
        
        static std::vector<byte_t> encrypt(
            const byte_t* data,
            const unsigned int& size,
            const CryptoPP::Integer& n,
            const CryptoPP::Integer& e,
            const bool& use_ecb = false
        );

        static std::pair<std::vector<byte_t>, std::vector<byte_t>> encrypt_half_split(
            const byte_t* data,
            unsigned size,
            const CryptoPP::Integer& n,
            const CryptoPP::Integer& e,
            unsigned short& padding_length
        );

        static std::vector<byte_t> decrypt(
            const byte_t* encrypted_data,
            const unsigned int& encrypted_data_size,
            const CryptoPP::Integer& n,
            const CryptoPP::Integer& d,
            const bool& use_ecb = false
        );

        static std::vector<byte_t> decrypt_half_join(
            const byte_t* first_half,
            const byte_t* second_half,
            unsigned cipher_pairs,
            unsigned short padding_length,
            const CryptoPP::Integer& n,
            const CryptoPP::Integer& d
        );
        
    private:
        static std::vector<byte_t> encrypt_block(
            const byte_t* m,
            const unsigned int& in_block_size,
            const CryptoPP::Integer& e,
            const CryptoPP::Integer& n
        );

        static std::vector<byte_t> decrypt_block(
            const byte_t* const c,
            const unsigned int& out_block_size,
            const CryptoPP::Integer& d,
            const CryptoPP::Integer& n
        );
};