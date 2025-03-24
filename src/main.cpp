#include <iostream>
#include <png.hpp>
#include <vector>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <fftw3.h>

#ifndef IMAGE_DIR
#define IMAGE_DIR "../example5.png"
#endif

int main() {
    try {
        PNGfile img(IMAGE_DIR);
        std::cout << img << std::endl;
    }
    catch (const PNGfile::Exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }

    const sf::Image image(IMAGE_DIR);
    unsigned int width = image.getSize().x;
    unsigned int height = image.getSize().y;

    sf::RenderWindow window(sf::VideoMode({2 * width, height}), "Obraz");

    fftw_complex* complexData = fftw_alloc_complex(width * height);
    fftw_complex* fft = fftw_alloc_complex(width * height);

    // fftw_plan plan = fftw_plan_dft_1d(width * height, complexData, fft, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan plan = fftw_plan_dft_2d(width, height, complexData, fft, FFTW_FORWARD, FFTW_ESTIMATE);

    double maxVal = 0.0;
    for (unsigned int i = 0, k = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++) {
            double r = static_cast<double>(image.getPixel({j, i}).r);
            double g = static_cast<double>(image.getPixel({j, i}).g);
            double b = static_cast<double>(image.getPixel({j, i}).b);

            complexData[k][0] = (r + g + b) / 3.0f;
            complexData[k++][1] = 0.0;
        }

    fftw_execute(plan);
    fftw_destroy_plan(plan);

    double magnitudes[width * height];
    sf::Image fftImage({width, height});
    for (unsigned int i = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++) {
            double real = fft[i * width + j][0];
            double imag = fft[i * width + j][1];
            double grey = std::sqrt(real * real + imag * imag);
            magnitudes[i * width + j] = grey;
            if (maxVal < grey) maxVal = grey;
        }

    for (unsigned int i = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++) {
            unsigned char grey = static_cast<unsigned char>(255.0 * (std::log(magnitudes[i * width + j]) / std::log(maxVal)));
            fftImage.setPixel({(j + width / 2) % width, (i + height / 2) % height }, sf::Color(grey, grey, grey));
        }
    
    const sf::Texture texture(image);
    const sf::Texture fftTexture(fftImage);
    sf::Sprite sprite(texture);
    sf::Sprite fftSprite(fftTexture);

    fftSprite.move({static_cast<float>(width), 0});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.draw(fftSprite);
        window.display();
    }    

    fftw_free(complexData);
    fftw_free(fft);
}