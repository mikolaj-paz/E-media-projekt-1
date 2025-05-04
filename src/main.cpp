#include <iostream>
#include <png.hpp>
#include <vector>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <fftw3.h>

#ifndef IMAGE_DIR
#define IMAGE_DIR "../example3.png"
#endif

int main() {
    try {
        PNGfile img(IMAGE_DIR);
        std::cout << img << std::endl;
        img.modify("../test.png");

        PNGfile img_modified("../test.png");
        std::cout << img_modified << std::endl;

        img.anonymize("../test.png", "../test_anonymized.png");
        PNGfile img_anonymized("../test_anonymized.png");
        std::cout << img_anonymized << std::endl;

        img.show();
    }
    catch (const PNGfile::Exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }

    sf::Image image(IMAGE_DIR);
    unsigned int width = image.getSize().x;
    unsigned int height = image.getSize().y;

    sf::RenderWindow window(sf::VideoMode({2 * width, 2 * height}), "DFT");

    fftw_complex* complexData = fftw_alloc_complex(width * height);
    fftw_complex* fft = fftw_alloc_complex(width * height);

    fftw_plan plan = fftw_plan_dft_2d(height, width, complexData, fft, FFTW_FORWARD, FFTW_ESTIMATE);

    for (unsigned int i = 0, k = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++) {
            double r = static_cast<double>(image.getPixel({j, i}).r);
            double g = static_cast<double>(image.getPixel({j, i}).g);
            double b = static_cast<double>(image.getPixel({j, i}).b);

            complexData[k][0] = 0.2126 * r + 0.7152 * g + 0.0722 * b; // RGB do Luminance
            complexData[k][1] = 0.0;
            
            unsigned char luminance = static_cast<unsigned char>(complexData[k++][0]);
            image.setPixel({j, i}, sf::Color(luminance, luminance, luminance));
        }

    fftw_execute(plan);
    fftw_destroy_plan(plan);

    double magnitudes[width * height];
    double phases[width * height];

    sf::Image fftImage({width, height});
    sf::Image fftShiftImage({width, height});
    sf::Image fftPhaseImage({width, height});

    double maxMagVal = 0.0;
    double maxPhaVal = 0.0;
    for (unsigned int i = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++) {
            double real = fft[i * width + j][0];
            double imag = fft[i * width + j][1];
            double grey = std::sqrt(real * real + imag * imag);
            double phase = std::atan(imag / real);
            magnitudes[i * width + j] = grey;
            phases[i * width + j] = phase;
            if (maxMagVal < grey) maxMagVal = grey;
            if (maxPhaVal < phase) maxPhaVal = phase;
        }

    for (unsigned int i = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++) {
            unsigned char grey = static_cast<unsigned char>(255.0 * std::log10(magnitudes[i * width + j]) / std::log10(maxMagVal));
            unsigned char phase = static_cast<unsigned char>(255.0 * std::log10(phases[i * width + j]) / std::log10(maxPhaVal));
            fftImage.setPixel({j, i}, sf::Color(grey, grey, grey));
            fftShiftImage.setPixel({ (j + width / 2) % width, (i + height / 2) % height }, sf::Color(grey, grey, grey));
            fftPhaseImage.setPixel({ (j + width / 2) % width, (i + height / 2) % height }, sf::Color(phase, phase, phase));
        }
    
    const sf::Texture texture(image);
    sf::Sprite sprite(texture);

    const sf::Texture fftTexture(fftImage);
    const sf::Texture fftShiftTexture(fftShiftImage);
    const sf::Texture fftPhaseTexture(fftPhaseImage);

    sf::Sprite fftSprite(fftTexture);
    sf::Sprite fftShiftSprite(fftShiftTexture);
    sf::Sprite fftPhaseSprite(fftPhaseTexture);

    fftSprite.setPosition({static_cast<float>(width), static_cast<float>(height)});
    fftShiftSprite.setPosition({static_cast<float>(width), 0});
    fftPhaseSprite.setPosition({0, static_cast<float>(height)});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.draw(fftSprite);
        window.draw(fftShiftSprite);
        window.draw(fftPhaseSprite);
        window.display();
    }    

    fftw_free(complexData);
    fftw_free(fft);
}