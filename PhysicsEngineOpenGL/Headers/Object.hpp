#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "SFMLObjects.hpp"
#include <string>

class Object {
public:
    Object();
    Object(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, float omega, float inertia);
    Object(float x, float y, float vx, float vy, float ax, float ay, float mass, float width, float height, float omega, float inertia);
    void applyForce(sf::Vector2f force);
    sf::FloatRect getGlobalBounds();
    float x, y; // position
    float vx, vy; // velocity
    float ax, ay; // acceleration
    float mass, omega, inertia, radius, width, height, size, collisionX, collisionY;
    std::string shape;
    sf::Vector2f position;
};

#endif /* OBJECT_HPP */
