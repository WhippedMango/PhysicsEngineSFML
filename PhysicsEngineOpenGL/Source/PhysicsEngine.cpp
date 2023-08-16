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
    if (magnitude <= 0) {
        return;
    }
    springDirection.x /= magnitude;
    springDirection.y /= magnitude;
    float displacement = magnitude - mouseDragDistance;
    sf::Vector2f springForce = -springConstant * displacement * springDirection;

    // Update acceleration based on spring force
    object.ax = springForce.x / object.mass;
    object.ay = springForce.y / object.mass;

    // Update velocity based on acceleration
    float dt = 1.0f / 60.0f;
    object.vx += object.ax * dt;
    object.vy += object.ay * dt;

    // Update position based on velocity
    object.x += object.vx * dt + 0.5 * object.ax * dt * dt;
    object.y += object.vy * dt + 0.5 * object.ay * dt * dt;
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

void PhysicsEngine::handleCollision(Object& object1, Object& object2, float COR) {
    // Calculate relative velocity
    
    float damp = 0.8f;

    sf::Vector2f relVelocity = sf::Vector2f(object2.vx - object1.vx, object2.vy - object1.vy);
    
    // Calculate the normal vector
    sf::Vector2f normal = sf::Vector2f(object2.x - object1.x, object2.y - object1.y);
    float distance = sqrt(normal.x * normal.x + normal.y * normal.y);
    normal /= distance; // Normalize
    
    // Calculate the impulse
    float impulseMagnitude = -COR * (1 + damp) * (relVelocity.x * normal.x + relVelocity.y * normal.y) /
                             (1 / object1.mass + 1 / object2.mass);
    
    // Apply the impulse to the velocities, taking into account damping
    object1.vx -= impulseMagnitude * normal.x / object1.mass;
    object1.vy -= impulseMagnitude * normal.y / object1.mass;
    object2.vx += impulseMagnitude * normal.x / object2.mass;
    object2.vy += impulseMagnitude * normal.y / object2.mass;
    
    // Apply damping to the velocities
    float dampingFactor = 0.95; // Adjust this value as needed
    object1.vx *= dampingFactor;
    object1.vy *= dampingFactor;
    object2.vx *= dampingFactor;
    object2.vy *= dampingFactor;
    
    // Separate the objects to avoid overlap
    float overlap = (object1.radius + object2.radius) - distance;
    object1.x -= overlap * normal.x * 0.5;
    object1.y -= overlap * normal.y * 0.5;
    object2.x += overlap * normal.x * 0.5;
    object2.y += overlap * normal.y * 0.5;
}


void PhysicsEngine::handleCollisionWindow(Object& object, float windowWidth, float windowHeight, float COR) {
    float restitution = COR; // Coefficient of restitution
    float mass = object.mass;
    
    if (object.y - object.radius < 0) {
        object.y = object.radius;
        object.vy = -object.vy * restitution;
    } else if (object.y + object.radius > windowHeight) {
        object.y = windowHeight - object.radius;
        object.vy = -object.vy * restitution;
    }
    
    if (object.x - object.radius < 0) {
        object.x = object.radius;
        object.vx = -object.vx * restitution;
    } else if (object.x + object.radius > windowWidth) {
        object.x = windowWidth - object.radius;
        object.vx = -object.vx * restitution;
    }
}

