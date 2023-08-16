#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "Object.hpp"
#include "PhysicsEngine.hpp"
#include "PhysicsConstants.hpp"
#include "RandomPosition.hpp"
#include <SFML/Graphics.hpp>

#include <vector>

class Block : public Object {
public:
    Block();
    Block(float x, float y, float vx, float vy, float ax, float ay, float mass, float width, float height, float omega, float inertia, PhysicsEngine& physics);
    void update(float timestep, float windowWidth, float windowHeight, float COR);
    void collide(Object object, float COR);
    sf::FloatRect getGlobalBounds();
    void applyForce(sf::Vector2f force);
    void blockHere(Object object1, Object object2, sf::RenderWindow &window);

private:
    PhysicsEngine physicsEngine;
};

#endif /* BLOCK_HPP */
