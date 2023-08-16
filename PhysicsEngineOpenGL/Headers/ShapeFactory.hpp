#ifndef SHAPEFACTORY_HPP
#define SHAPEFACTORY_HPP

#include "SFMLObjects.hpp"
#include <random>

class ShapeFactory : public sf::Drawable {
public:
    static std::vector<sf::CircleShape> createCircleShape(int numShapes, float radius, sf::Vector2f position);
    static std::vector<sf::RectangleShape> createSquareShape(int numShapes, sf::Vector2f position);
    static sf::Color randomColour();
};

#endif /* SHAPEFACTORY_HPP */
