#include <iostream>
#include <png.hpp>
#include <vector>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <fftw3.h>

#ifndef IMAGE_DIR
#define IMAGE_DIR "../example3.png"
#endif

void createFFT(const sf::Image& image, sf::Image& outputImage) {
    unsigned int width = image.getSize().x;
    unsigned int height = image.getSize().y;

    double input[width * height];
    fftw_complex output[width * height];

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            sf::Color color = image.getPixel({x, y});
            input[y * width + x] = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
        }
    }

    fftw_plan plan = fftw_plan_dft_r2c_2d(height, width, input, output, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    // Obliczenie amplitudy spektrum i znalezienie wartości maksymalnej
    double magnitude[width * height];
    for (unsigned int i = 0; i < width * height; i++) {
        double real = output[i][0];
        double imag = output[i][1];
        magnitude[i] = log(1 + sqrt(real * real + imag * imag));
    }

    // Zapis wartości do obrazu SFML
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            uint8_t intensity = magnitude[y * width + x];
            outputImage.setPixel({x, y}, sf::Color(intensity, intensity, intensity));
            // std::cout << static_cast<int>(intensity) << " ";
        }
    }
}

int main() {
    sf::Vector2u imageSize;
    try {
        PNGfile img(IMAGE_DIR);
        std::cout << img << std::endl;

        imageSize.x = img.width();
        imageSize.y = img.height();
    }
    catch (const PNGfile::Exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }

    sf::RenderWindow window(sf::VideoMode(imageSize), "Obraz");
    const sf::Image image(IMAGE_DIR);

    sf::Image fftImage({imageSize.x, imageSize.y});
    createFFT(image, fftImage);
    
    const sf::Texture texture(fftImage);
    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}