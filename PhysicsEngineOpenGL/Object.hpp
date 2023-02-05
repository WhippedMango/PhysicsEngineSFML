//
//  Object.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 19/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

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
//        this->position.x = x;
//        this->position.y = y;
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

//    virtual sf::FloatRect getAABB() = 0;
//    virtual std::vector<sf::Vector2f> getNormals() = 0;
//    sf::FloatRect getAABB() {
//        if(typeid(*this) == typeid(sf::CircleShape)){
//            sf::FloatRect aabb = sf::FloatRect(x - radius, y - radius, 2 * radius, 2 * radius);
//            return aabb;
//        } else if(typeid(*this) == typeid(sf::RectangleShape)){
//            sf::FloatRect aabb = sf::FloatRect(x, y, width, height);
//            return aabb;
//        }
//    }
//    std::vector<sf::Vector2f> getNormals() {
//        if(typeid(*this) == typeid(sf::CircleShape)){
//            std::vector<sf::Vector2f> normals;
//            return normals;
//        } else if(typeid(*this) == typeid(sf::RectangleShape)){
//            std::vector<sf::Vector2f> normals;
//            normals.push_back(sf::Vector2f(1.0f, 0.0f));
//            normals.push_back(sf::Vector2f(0.0f, 1.0f));
//            normals.push_back(sf::Vector2f(-1.0f, 0.0f));
//            normals.push_back(sf::Vector2f(0.0f, -1.0f));
//            return normals;
//        }
//    }
