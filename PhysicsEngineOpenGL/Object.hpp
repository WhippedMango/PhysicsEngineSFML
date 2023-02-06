#ifndef OBJECT_HPP
#define OBJECT_HPP


#include "SFMLObjects.hpp"

#include <iostream>

class Object {
public:
    float x, y; // position
    float vx, vy; // velocity
    float ax, ay; // acceleration
    float mass, omega, inertia, radius, width, height, size, collisionX, collisionY;
    std::string shape;
    sf::Vector2f position;
    
    Object(){}
    Object(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, float omega, float inertia) {
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
        this->ax = ax;
        this->ay = ay;
        this->collisionX = 0;
        this->collisionY = 0;
        this->shape = "circle";
        this->mass = mass;
        this->size = radius;
        this->radius = radius;
        this->omega = omega;
        this->inertia = inertia;
    }
    Object(float x, float y, float vx, float vy, float ax, float ay, float mass, float width, float height, float omega, float inertia) {
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
        this->ax = ax;
        this->ay = ay;
        this->collisionX = 0;
        this->collisionY = 0;
        this->size = width*height;
        this->shape = "square";
        this->mass = mass;
        this->width = width*height;
        this->height = height;
        this->omega = omega;
        this->inertia = inertia;
        position.x = x;
        position.y = y;
    }
    void applyForce(sf::Vector2f force) {
        ax += force.x / mass;
        ay += force.y / mass;
    }
    sf::FloatRect getGlobalBounds() {
        return sf::FloatRect(x * radius, y * radius, 2 * radius, 2 * radius);
    }
};

#endif /* Object_hpp */
