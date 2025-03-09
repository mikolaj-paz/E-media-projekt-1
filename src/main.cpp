#include <iostream>
#include <png.hpp>

int main() {
    PNGfile img("../example.png");
    std::cout << img << std::endl;
}