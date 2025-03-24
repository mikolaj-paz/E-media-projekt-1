#include <iostream>
#include <png.hpp>
#include <vector>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <fftw3.h>

#ifndef IMAGE_DIR
#define IMAGE_DIR "../example2.png"
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

    const sf::Image image("../cln1.gif");
    unsigned int width = image.getSize().x;
    unsigned int height = image.getSize().y;

    sf::RenderWindow window(sf::VideoMode({width, height}), "Obraz");

    fftw_complex* complexData = fftw_alloc_complex(width * height);
    fftw_complex* fft = fftw_alloc_complex(width * height);

    fftw_plan plan = fftw_plan_dft_1d(width * height, complexData, fft, FFTW_FORWARD, FFTW_ESTIMATE);

    for (unsigned int i = 0, k = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++) {
            complexData[k][0] = static_cast<double>(image.getPixel({j, i}).r);
            complexData[k++][1] = 0.0;
        }

    fftw_execute(plan);
    fftw_destroy_plan(plan);

    // double max = 0.0, min = 99999999.0, avg = 0.0; 
    // for (int i = 0; i < width * height; i++) {
    //     double val = std::log(1.0 + fft[i][0]);
    //     if (max < val) max = val;
    //     if (min > val) min = val;
    //     avg += val;
    // }
    // avg /= static_cast<double>(width * height);
    // std::cout << "max: " << max << " min: " << min << " avg: " << avg << std::endl;

    sf::Image fftImage({width, height});
    for (unsigned int i = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++) {
            double real = fft[i * width + j][0] / static_cast<double>(width * height);
            double imag = fft[i * width + j][1] / static_cast<double>(width * height);
            unsigned char grey = static_cast<unsigned char>(std::sqrt(real * real + imag * imag));
            // std::cout << static_cast<int>(grey) << " ";
            fftImage.setPixel({j, i}, sf::Color(grey, grey, grey));
        }

    std::cout << std::endl;

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

    fftw_free(complexData);
    fftw_free(fft);
}