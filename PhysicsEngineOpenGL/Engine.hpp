#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "RandomPosition.hpp"
#include "SFMLObjects.hpp"
#include "PhysicsEngine.hpp"
#include "PhysicsConstants.hpp"
#include "Ball.hpp"
#include "Block.hpp"
#include "ShapeFactory.hpp"
#include "Quadtree.hpp"
#include "KeyEventHandler.hpp"
#include "MouseEventHandler.hpp"
#include "Spring.hpp"


#include <iostream>

class Engine {
    
private:
    std::vector<Ball> balls;
    std::vector<sf::CircleShape> shapes;
    PhysicsEngine physics;
    Quadtree* quadtree;
    int maxObjects;
    float bounce = 50; // Height of bounce
    MouseHandler mouseHandler;
    KeyHandler keyHandler;
    
public:
    Engine(sf::RenderWindow &window) {
        // Initialise the physics engine
        physics = PhysicsEngine();
        
        // Set up the quadtree
        int maxObjects = 30;
        quadtree = new Quadtree(maxObjects, sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
        
        // Allocate size of the balls vector
        balls.reserve(maxObjects);
    }
    
    ~Engine() {
        balls.clear();
        shapes.clear();
        delete quadtree;
    }
    
    void run(sf::RenderWindow &window) {
        // Initialise KE value for debug
        float ke = 0;
        
        while (window.isOpen()) {
            sf::Clock clock;
            // Handle events
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                keyHandler.handleKeyEvent(event, balls, shapes, quadtree, window, physics);
                mouseHandler.handleMouseEvent(event, balls, shapes, window, bounce);
            }
            
            // Clear the previous frame
            window.clear();
            
            for (int i = 0; i < balls.size(); i++) {
                // Add the kinetic energy
                ke += 0.5 * balls[i].mass / (balls[i].vx * balls[i].vx + balls[i].vy * balls[i].vy);
                // Update ball position
                balls[i].ay = GRAVITY;
                balls[i].update(TIMESTEP/4, window.getSize().x, window.getSize().y, 0.9);
                
                std::vector<Object*> nearbyObjects = quadtree->retrieve(balls[i].getGlobalBounds());
                // Check for collision between balls and handle
                for (int j = i + 1; j < balls.size(); j++) {
                    if (physics.checkCollision(balls[i], balls[j])) {
                        physics.handleCollision(balls[i], balls[j], 0.9);
                    }
                }
                
                shapes[i].setPosition(balls[i].x - balls[i].radius/2, balls[i].y - balls[i].radius/2);
                window.draw(shapes[i]);
            }
            
            // Debug print statement of Kinetic Energy
            std::cout << ke << std::endl;
            ke = 0;
            
            // Display the window contents
            window.display();
        }
    }
};

#endif
