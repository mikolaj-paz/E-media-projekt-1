#include <iostream>
#include <png.hpp>

#ifndef IMAGE_DIR
#define IMAGE_DIR "../example.png"
#endif

int main() {
    try {
        PNGfile img(IMAGE_DIR);
        std::cout << img << std::endl;
        img.encrypt("../test_encrypted.png");
        std::cout << img << std::endl;
    }
    catch (const PNGfile::Exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}