#include <iostream>
#include <png.hpp>
#include <cryptopp/integer.h>

#ifndef IMAGE_DIR
#define IMAGE_DIR "../example4.png"
#endif

int main() {
    try {
        PNGfile img(IMAGE_DIR);
        std::cout << img << std::endl;

        CryptoPP::Integer n, d, e;
        img.encrypt("../test_encrypted.png", n, d, e);
        
        PNGfile encrypted("../test_encrypted.png");
        std::cout << encrypted << std::endl;
        img.decrypt("../test_decrypted.png", n, d);
    }
    catch (const PNGfile::Exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}