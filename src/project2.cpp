#include <iostream>
#include <chrono>

#include <png.hpp>
#include <cryptopp/integer.h>

#ifndef IMAGE_DIR
#define IMAGE_DIR "../example7.png"
#endif

int main() {
    try {
        PNGfile img(IMAGE_DIR);
        std::cout << img << std::endl;

        CryptoPP::Integer n, d, e;

        auto start = std::chrono::high_resolution_clock::now();
        img.encrypt(
            "../test_encrypted.png",
            n, d, e,
            /*encrypt_compressed=*/true,
            /*use_ecb=*/false
        );
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Encryption time: " << duration.count() / 1e3 << " s" << std::endl << std::endl;
        
        PNGfile encrypted("../test_encrypted.png");

        start = std::chrono::high_resolution_clock::now();
        encrypted.decrypt(
            "../test_decrypted.png",
            n, d,
            /*decrypt_compressed=*/true,
            /*use_ecb*/false
        );
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Decryption time: " << duration.count() / 1e3 << " s" << std::endl << std::endl;

        std::cout << encrypted << std::endl;
    }
    catch (const PNGfile::Exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}