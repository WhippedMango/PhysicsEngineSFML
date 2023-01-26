//
//  PhysicsEngine.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 19/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP
#include "Object.hpp"

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();
    void applyGravity(Object &object);
    void updatePosition(Object &object, float timestep);
    void updateVelocity(Object &object, float timestep);
    bool checkCollision(Object &object1, Object &object2);
    void handleCollision(Object &object1, Object &object2, float COR);
    void handleCollisionWindow(Object &object, float windowWidth, float windowHeight, float COR);
};

#endif /* PhysicsEngine_hpp */
