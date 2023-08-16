#ifndef BALL_HPP
#define BALL_HPP

#include "Object.hpp"
#include "PhysicsEngine.hpp"
#include "PhysicsConstants.hpp"
#include "RandomPosition.hpp"
#include <SFML/Graphics.hpp>

#include <vector>

class Ball : public Object {
public:
    Ball();
    Ball(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, float omega, float inertia, PhysicsEngine& physics);
    void update(float timestep, float windowWidth, float windowHeight, float COR);
    void collide(Object object, float COR);
    sf::FloatRect getGlobalBounds();
    void applyForce(sf::Vector2f force);
    void bounce(float COR, float bounce);
    void spring(float springConstant, float mouseDragDistance, sf::Vector2f currentPos, sf::Vector2f startPos);
    void ballHere(Object object1, Object object2, sf::RenderWindow &window);

private:
    PhysicsEngine physicsEngine;
};

#endif /* BALL_HPP */
