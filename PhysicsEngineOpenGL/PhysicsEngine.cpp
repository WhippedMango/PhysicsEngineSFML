#include "PhysicsEngine.hpp"
#include "PhysicsConstants.hpp"

#include <math.h>
#include <SFML/System/Vector2.hpp>

PhysicsEngine::PhysicsEngine() {
    // constructor code
}

PhysicsEngine::~PhysicsEngine() {
    // destructor code
}

void PhysicsEngine::applyGravity(Object &object) {
    object.ay = GRAVITY;
}

void PhysicsEngine::updatePosition(Object &object, float timestep) {
    object.x += object.vx * timestep + 0.5 * object.ax * timestep * timestep;
    object.y += object.vy * timestep + 0.5 * object.ay * timestep * timestep;
}

void PhysicsEngine::updateVelocity(Object &object, float timestep, float COR) {
    object.vx += object.ax * timestep * COR;
    object.vy += object.ay * timestep * COR;
}

void PhysicsEngine::bounce(Object &object, float COR, float bounce) {
    // Caalculate the acceleration of the object after the bounce
    float ay = -COR * object.vy / object.mass;
    
    // Update the acceleration of the object
    object.ay = ay - bounce;
    object.vy = -COR * object.vy + object.ay;
}

void PhysicsEngine::spring(Object &object, float springConstant, float mouseDragDistance, sf::Vector2f currentPos, sf::Vector2f startPos) {
    object.x = currentPos.x;
    object.y = currentPos.y;
    sf::Vector2f springDirection = sf::Vector2f(currentPos.x - startPos.x, currentPos.y - startPos.y);
    float magnitude = sqrt(springDirection.x * springDirection.x + springDirection.y * springDirection.y);
    if (magnitude <= 0){
        return;
    }
    springDirection.x /= magnitude;
    springDirection.y /= magnitude;
    float displacement = magnitude - mouseDragDistance;
    sf::Vector2f springForce = -springConstant * displacement * springDirection;
    
    float dt = 1.0f / 60.0f;
    object.vx += object.ax * dt;
    object.vy += object.ay * dt;
    object.x += object.vx * dt + 0.5 * object.ax * dt * dt;
    object.y += object.vy * dt + 0.5 * object.ay * dt * dt;
    object.ax = -springForce.x / object.mass;
    object.ay = -springForce.y / object.mass;
}

bool PhysicsEngine::checkCollision(Object &object1, Object &object2) {
    if (object1.shape == "circle" && object2.shape == "circle") {
        float distx = object1.x - object2.x;
        float disty = object1.y - object2.y;
        float distance = sqrt(distx * distx + disty * disty);
        return distance < object1.radius + object2.radius;
    } else if (object1.shape == "square" && object2.shape == "square") {
        float distx = object1.x - object2.x;
        float disty = object1.y - object2.y;
        return abs(distx) < (object1.size + object2.size) / 2 && abs(disty) < (object1.size + object2.size) / 2;
    } else {
        float distx = abs(object1.x - object2.x);
        float disty = abs(object1.y - object2.y);
        if (object1.shape == "circle") {
            std::swap(object1, object2);
        }
        return distx <= object2.size / 2 + object1.radius && disty <= object2.size / 2 + object1.radius;
    }
}

void PhysicsEngine::handleCollision(Object &object1, Object &object2, float COR) {
    // Get the normal vector between the objects
    float normx = object2.x  - object1.x;
    float normy = object2.y - object1.y;
    float new_mag = sqrt(normx*normx + normy*normy);
    float damp = 0.7;
    normx /= new_mag;
    normy /= new_mag;
    
    float relNormalVel = (object1.vx - object2.vx) * normx + (object1.vy - object2.vy) * normy;
    
    // Get the tangent vector
    float tanx = -normy;
    float tany = normx;
    
    if (relNormalVel > 0) {
        // Calculate the new velocity based on conservation of momentum
        float mass1 = object1.mass;
        float mass2 = object2.mass;
        float total_mass = mass1 + mass2;
        float impulse = 2 * mass2 * relNormalVel / total_mass * COR;
        object1.vx -= impulse * normx / mass1;
        object1.vy -= impulse * normy / mass1;
        object2.vx += impulse * normx / mass2;
        object2.vy += impulse * normy / mass2;
        
        float dist = object2.radius + object1.radius - new_mag;
        object1.x -= normx * dist / 2;
        object1.y -= normy * dist / 2;
        object2.x += normx * dist / 2;
        object2.y += normy * dist / 2;
    }
}

void PhysicsEngine::handleCollisionWindow(Object &object, float windowWidth, float windowHeight, float COR) {
    float mass = object.mass;  // added mass of the object
    if (object.y - object.radius < 0) {
        object.y = object.radius;
        object.vy = -object.vy * COR * mass;  // reversed the velocity in the y direction and multiplied by mass
    } else if (object.y + object.radius > windowHeight) {
        object.y = windowHeight - object.radius;
        object.vy = -object.vy * COR * mass;  // reversed the velocity in the y direction and multiplied by mass
    }
    if (object.x - object.radius < 0) {
        object.x = object.radius;
        object.vx = -object.vx * COR * mass;  // reversed the velocity in the x direction and multiplied by mass
    } else if (object.x + object.radius > windowWidth) {
        object.x = windowWidth - object.radius;
        object.vx = -object.vx * COR * mass;  // reversed the velocity in the x direction and multiplied by mass
    }
}
