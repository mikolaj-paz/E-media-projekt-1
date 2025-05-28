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
        
        static void encrypt(
            byte_t* data,
            unsigned int& size,
            Integer& n,
            Integer& d,
            Integer& e,
            const bool& use_ecb = false
        );
        
    private:
        static void encrypt_block(
            byte_t* const m, 
            byte_t* const c,
            const unsigned int& in_block_size,
            Integer& e,
            Integer& n
        );
};