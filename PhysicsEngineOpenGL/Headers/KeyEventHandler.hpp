#ifndef KEYEVENTHANDLER_HPP
#define KEYEVENTHANDLER_HPP

#include <SFML/Window.hpp>
#include "Quadtree.hpp"
#include "Ball.hpp"
#include "ShapeFactory.hpp"
#include "PhysicsEngine.hpp"

class KeyHandler {
public:
    static void handleKeyEvent(sf::Event event, std::vector<Ball>& balls, std::vector<sf::CircleShape>& shapes, Quadtree* quadtree, sf::RenderWindow& window, PhysicsEngine physics);
};

#endif /* KEYEVENTHANDLER_HPP */
