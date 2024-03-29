#include "ShapeFactory.hpp"

std::vector<sf::CircleShape> ShapeFactory::createCircleShape(int numShapes, float radius, sf::Vector2f position) {
    std::vector<sf::CircleShape> shapes;
    for (int i = 0; i < numShapes; i++) {
        sf::CircleShape shape(radius);
        shape.setFillColor(randomColour());
        shape.setPosition(position);
        shape.setOrigin(radius / 2, radius / 2);
        shapes.push_back(shape);
    }
    return shapes;
}

std::vector<sf::RectangleShape> ShapeFactory::createSquareShape(int numShapes, sf::Vector2f position) {
    std::vector<sf::RectangleShape> shapes;
    for (int i = 0; i < numShapes; i++) {
        sf::RectangleShape shape(sf::Vector2f(128.0f, 128.0f));
        shape.setFillColor(randomColour());
        shape.setPosition(position);
        shapes.push_back(shape);
    }
    return shapes;
}

sf::Color ShapeFactory::randomColour() {
    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create distributions for the color components
    std::uniform_int_distribution<> redDist(0, 255);
    std::uniform_int_distribution<> greenDist(0, 255);
    std::uniform_int_distribution<> blueDist(0, 255);

    return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
}
