#include <iostream>
#include <png.hpp>

int main() {
    try {
        PNGfile img("../example.png");
        std::cout << img << std::endl;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}