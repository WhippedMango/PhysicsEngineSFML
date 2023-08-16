#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "RandomPosition.hpp"
#include "SFMLObjects.hpp"
#include "PhysicsEngine.hpp"
#include "PhysicsConstants.hpp"
#include "Ball.hpp"
#include "ShapeFactory.hpp"
#include "Quadtree.hpp"
#include "KeyEventHandler.hpp"
#include "MouseEventHandler.hpp"

#include <iostream>

class Engine {
private:
    std::vector<Ball> balls;
    std::vector<sf::CircleShape> shapes;
    PhysicsEngine physics;
    Quadtree* quadtree;
    int maxObjects;
    float bounce;
    MouseEventHandler mouseHandler;
    KeyHandler keyHandler;

public:
    Engine(sf::RenderWindow &window);
    ~Engine();
    void run(sf::RenderWindow &window);
};

#endif /* ENGINE_HPP */
