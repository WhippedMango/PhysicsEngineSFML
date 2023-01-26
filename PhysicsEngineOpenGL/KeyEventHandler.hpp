//
//  KeyEventHandler.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 24/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#ifndef KEYEVENTHANDLER_hpp
#define KEYEVENTHANDLER_hpp

#include "Engine.hpp"

class KeyEventHandler {
public:
    static void handleKeyPressed(sf::Event event, std::vector<Ball>&balls, std::vector<sf::CircleShape>&shapes, Quadtree* quadtree, sf::RenderWindow &window, PhysicsEngine physics) {
        float windowx = window.getSize().x;
        float windowy = window.getSize().y;
        if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {            if (balls.size() < balls.capacity()) {
                quadtree->insert(&balls.back());
                sf::Vector2f randomPosition = getRandomPosition(window);
                float new_x = randomPosition.x;
                float new_y = randomPosition.y;
                float new_vx = rand() % 20 - 10;
                float new_vy = rand() % 20 - 10;
                Ball new_ball;
                for (int i = 0; i < balls.size(); i++) {
                    std::vector<Object*> nearbyObjects = quadtree->retrieve(shapes[i].getGlobalBounds());
                    for (int j = i + 1; j < nearbyObjects.size(); j++) {
                        if (physics.checkCollision(new_ball, *nearbyObjects[j])) {
                            new_ball.x += new_ball.radius/2;
                        }
                    }
                }
                new_ball = Ball(new_x, new_y, new_vx, new_vy, ax, ay, mass, radius, omega, inertia, physics);
                balls.push_back(new_ball);
                shapes.push_back(ShapeFactory::createCircleShape(1, radius, sf::Vector2f(new_x, new_y))[0]);
            }
        }
    }
};


#endif /* KeyEventHandler_hpp */
