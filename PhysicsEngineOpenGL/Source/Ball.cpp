#include "Ball.hpp"

Ball::Ball() {};
Ball::Ball(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, float omega, float inertia, PhysicsEngine& physics) : Object(x, y, vx, vy, ax, ay, mass, radius, omega, inertia){
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
    this->position.x = x;
    this->position.y = y;
}

void Ball::update(float timestep, float windowWidth, float windowHeight, float COR) {
    physicsEngine.applyGravity(*this);
    physicsEngine.updateVelocity(*this, timestep, COR);
    physicsEngine.updatePosition(*this, timestep);
    physicsEngine.handleCollisionWindow(*this, windowWidth, windowHeight, COR);
    
}

void Ball::collide(Object object, float COR) {
    if (physicsEngine.checkCollision(*this, object)) {
        physicsEngine.handleCollision(*this, object, COR);
    }
}

sf::FloatRect Ball::getGlobalBounds() {
    return sf::FloatRect(x * radius, y * radius, 2 * radius, 2 * radius);
}

void Ball::applyForce(sf::Vector2f force) {
    ax += force.x / mass;
    ay += force.y / mass;
}

void Ball::bounce(float COR, float bounce) {
    physicsEngine.bounce(*this, COR, bounce);
}

void Ball::spring(float springConstant, float mouseDragDistance, sf::Vector2f currentPos, sf::Vector2f startPos) {
    physicsEngine.spring(*this, springConstant, mouseDragDistance, currentPos, startPos);
}

void Ball::ballHere(Object object1, Object object2, sf::RenderWindow &window){
    if (this->getGlobalBounds().contains(object1.size, object1.size)) {
        object2.x = getRandomPosition(window).x;
        object2.y = -object2.y;
        ballHere(object1, object2, window);
    }
    return 0;
}
