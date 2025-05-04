#include <png.hpp>
#include <SFML/Graphics.hpp>

void PNGfile::anonymize(const std::string& src, const std::string& dst) {
    sf::Image img(src);
    if (!img.saveToFile(dst))
        throw PNGfile::Exception("Couldn't save file during anonymization.");
}