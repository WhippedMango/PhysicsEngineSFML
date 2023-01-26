//
//  Block.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 25/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp

#include "Object.hpp"
#include "PhysicsEngine.hpp"
#include <SFML/Graphics.hpp>

#include <vector>

class Block : public Object {
public:
    std::vector<sf::RectangleShape>blocks;
    std::string shape;
    float size;
    Block() {};
    Block(float x, float y, float vx, float vy, float ax, float ay, float mass, float width, float height, float omega, float inertia, PhysicsEngine& physics) : Object(x, y, vx, vy, ax, ay, mass, width, height, omega, inertia){
        this->x = x;
        this->y = y;
        this->mass = mass;
        this->omega = omega;
        this->inertia = inertia;
        this->height = height;
        this->width = widt;h;
        this->size = width;
        this->shape = "square";
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
        return sf::FloatRect(x, y, width, height);
    }
    
//    std::vector<sf::Vector2f> getNormals() override {
//        return {
//            sf::Vector2f(1.0f, 0.0f),
//            sf::Vector2f(0.0f, 1.0f),
//            sf::Vector2f(-1.0f, 0.0f),
//            sf::Vector2f(0.0f, -1.0f)
//        };
//    }
    
private:
    PhysicsEngine physicsEngine;
};

#endif /* Block_hpp */
