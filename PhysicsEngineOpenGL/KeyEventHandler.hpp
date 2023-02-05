//
//  KeyEventHandler.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 24/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#ifndef KEYEVENTHANDLER_hpp
#define KEYEVENTHANDLER_hpp

#include <iostream>
#include <SFML/Window.hpp>
#include "Quadtree.hpp"
#include "Ball.hpp"
#include "ShapeFactory.hpp"
#include "PhysicsEngine.hpp"

class KeyHandler {
public:
    static void handleKeyEvent(sf::Event event, std::vector<Ball>&balls, std::vector<sf::CircleShape>&shapes, Quadtree* quadtree, sf::RenderWindow &window, PhysicsEngine physics) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
                std::cout << "key press detected" << std::endl;  // Debug print statement
                if (balls.size() < balls.capacity()) {
                    quadtree->insert(&balls.back());
                    sf::Vector2f randomPosition = getRandomPosition(window);
                    float new_x = randomPosition.x;
                    float new_y = randomPosition.y;
                    float new_vx = rand() % 20 - 10;
                    float new_vy = rand() % 20 - 10;
                    Ball new_ball(new_x, new_y, new_vx, new_vy, ax, ay, mass, radius, omega, inertia, physics);
                    // Use the Quadtree to determine if a shape will randomly spawn on another shape and if so, move it
                    for (int i = 0; i < balls.size(); i++) {
                        std::vector<Object*> nearbyObjects = quadtree->retrieve(balls[i].getGlobalBounds());
                        for (int j = i + 1; j < nearbyObjects.size(); j++) {
                            balls[i].ballHere(balls[j], new_ball, window);
                        }
                    }
                    balls.push_back(new_ball);
                    shapes.push_back(ShapeFactory::createCircleShape(1, radius, sf::Vector2f(new_x, new_y))[0]);
                }
            }
        }
    }
};


#endif /* KeyEventHandler_hpp */
