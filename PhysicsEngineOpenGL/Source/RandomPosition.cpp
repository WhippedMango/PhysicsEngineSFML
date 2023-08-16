#include "RandomPosition.hpp"

sf::Vector2f getRandomPosition(sf::RenderWindow& window) {
    float x = static_cast<float>(rand() % window.getSize().x);
    float y = static_cast<float>(rand() % (window.getSize().y / 4 * 3));
    return sf::Vector2f(x, y);
}
