#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP
#include "Object.hpp"

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();
    void applyGravity(Object &object);
    void updatePosition(Object &object, float timestep);
    void updateVelocity(Object &object, float timestep, float COR);
    void bounce(Object &object, float COR, float bounce);
    void spring(Object &object, float springConstant, float mouseDragDistance, sf::Vector2f currentPos, sf::Vector2f startPos);
    bool checkCollision(Object &object1, Object &object2);
    void handleCollision(Object &object1, Object &object2, float COR);
    void handleCollisionWindow(Object &object, float windowWidth, float windowHeight, float COR);
};

#endif /* PhysicsEngine_hpp */
