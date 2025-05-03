#include <png.hpp>
#include <SFML/Graphics.hpp>

void PNGfile::anonymize(const std::string& path) {
    sf::Image img("../test.png");
    if (!img.saveToFile(path))
        throw PNGfile::Exception("Nie udalo sie zapisac przy anonimizacji.");
}