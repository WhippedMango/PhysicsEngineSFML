#ifndef MOUSEEVENTHANDLER_HPP
#define MOUSEEVENTHANDLER_HPP

#include "SFMLObjects.hpp"
#include "Ball.hpp"
#include "PhysicsEngine.hpp"
#include <vector>
#include <cmath>
#include <iostream>

class MouseEventHandler {
private:
PhysicsEngine physics;
bool isDragging = false;
sf::Vector2f currentPos, startPos;
float mouseDragDistance = 0;
public:
    void handleMouseEvent(sf::Event event, std::vector<Ball> &balls, std::vector<sf::CircleShape> &shapes, sf::RenderWindow &window, float &bounce);
};

#endif
