//
//  Ball.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 19/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#ifndef BALL_HPP
#define BALL_HPP

#include "Object.hpp"
#include "PhysicsEngine.hpp"
#include "PhysicsConstants.hpp"
#include <SFML/Graphics.hpp>

#include <vector>

class Ball : public Object {
public:
    std::vector<sf::CircleShape>balls;
    std::string shape;
    Ball() {};
    Ball(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, float omega, float inertia, PhysicsEngine& physics) : Object(x, y, vx, vy, ax, ay, mass, radius, omega, inertia){
        this->x = x;
        this->y = y;
        this->ax = ax;
        this->ay = GRAVITY;
        this->mass = mass;
        this->radius = radius;
        this->omega = omega;
        this->inertia = inertia;
        this->shape = "circle";
        this->size = radius;
        this->vx = (rand() % 200) - 100;  // random value between -100 and 100
        this->vy = (rand() % 200) - 100;  // random value between -100 and 100
        physicsEngine = physics;
    }
    
    void update(float timestep, float windowWidth, float windowHeight, float COR) {
        physicsEngine.applyGravity(*this);
        physicsEngine.updateVelocity(*this, timestep);
        physicsEngine.updatePosition(*this, timestep);
        physicsEngine.handleCollisionWindow(*this, windowWidth, windowHeight, COR);
        
    }
    
    void collide(Object object, float COR){
        if (physicsEngine.checkCollision(*this, object)) {
            physicsEngine.handleCollision(*this, object, COR);
        }
    }
    
    sf::FloatRect getGlobalBounds() {
        return sf::FloatRect(x * radius, y * radius, 2 * radius, 2 * radius);
    }
    
private:
    PhysicsEngine physicsEngine;
};

#endif /* Ball_hpp */

//        void spawnBall(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, PhysicsEngine &physicsEngine) {
//            sf::CircleShape circle(radius);
//            circle.setPosition(x, y);
//            circle.setFillColor(sf::Color::Blue);
//            balls.push_back(circle);
//        }
