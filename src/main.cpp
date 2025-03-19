#include <iostream>
#include <png.hpp>

int main() {
    try {
        PNGfile img("../example2.png");
        std::cout << img << std::endl;
    }
    catch (const PNGfile::Exception& e) {
        std::cout << e.what() << std::endl;
    }


    // unsigned char sig[4] = {0x49, 0x44, 0x41, 0x54};

    // std::string a = "IDAT";
    // std::string b(reinterpret_cast<char*>(sig));

    // std::cout << (a == b) << std::endl;
}