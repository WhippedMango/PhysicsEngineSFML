//
//  Ball.cpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 19/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//


#include "Ball.hpp"

//#include "Ball.hpp"
//#include "Object.hpp"
//
//class Ball : public Object {
//public:
//    Ball() {};
//    Ball(float x, float y, float vx, float vy,  float ax, float ay, float mass, float radius) : Object(x, y, vx, vy, mass, radius){
//        this->x = x;
//        this->y = y;
//        this->mass = mass;
//        this->radius = radius;
//    }
//    float radius;
//    void update(float timestep, float windowWidth, float windowHeight) {
//        updateVelocity(timestep);
//        updatePosition(timestep);
//        handleCollision(windowWidth, windowHeight);
//    }
//    void updateVelocity(float timestep) {
//        vx += ax * timestep;
//        vy += ay * timestep;
//    }
//    void updatePosition(float timestep) {
//        x += vx * timestep;
//        y += vy * timestep;
//    }
//    void handleCollision(float windowWidth, float windowHeight) {
//        if (x - radius < 0) {
//            x = 0 + radius;
//            vx = -vx;  // reverse the velocity in the x direction
//        } else if (x + radius > windowWidth) {
//            x = windowWidth - radius;
//            vx = -vx;  // reverse the velocity in the x direction
//        }
//        if (y - radius < 0) {
//            y = 0 + radius;
//            vy = -vy;  // reverse the velocity in the y direction
//        } else if (y + radius > windowHeight) {
//            y = windowHeight - radius;
//            vy = -vy;  // reverse the velocity in the y direction
//        }
//    }
//};
