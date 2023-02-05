//
//  MouseEventHandler.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 24/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#ifndef MOUSEEVENTHANDLER_HPP
#define MOUSEEVENTHANDLER_HPP

#include "SFMLObjects.hpp"
#include "Ball.hpp"
#include "PhysicsEngine.hpp"
#include <vector>
#include <cmath>

class MouseHandler {
private:
PhysicsEngine physics;
bool isDragging = false;
sf::Vector2f currentPos, startPos;
float mouseDragDistance = 0;

public:
    static void handleMouseEvent(sf::Event event, std::vector<Ball> &balls, std::vector<sf::CircleShape> &shapes, sf::RenderWindow &window, float &bounce) {
        PhysicsEngine physics;
        static bool isDragging = false;
        static sf::Vector2f currentPos, startPos;
        static float mouseDragDistance = 0;
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            startPos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            isDragging = true;
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Right) {
                isDragging = false;
            }
        }
        else if (event.type == sf::Event::MouseMoved && isDragging) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            currentPos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
            mouseDragDistance = sqrt(pow(currentPos.x - startPos.x, 2) + pow(currentPos.y - startPos.y, 2));
            std::cout << "Mouse Drag Distance: " << mouseDragDistance << std::endl;
            for (int i = 0; i < balls.size(); i++) {
                if (shapes[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    for (int j = i; j < balls.size(); j++){
                        if (!physics.checkCollision(balls[i], balls[j]) || !physics.checkCollision(balls[i], balls[0])){
                            balls[i].spring(150, mouseDragDistance, currentPos, startPos);
                        }
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            isDragging = false;
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            for (int i = 0; i < balls.size(); i++) {
                if (shapes[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    balls[i].bounce(0.9, bounce);
                }
            }
        }
    }
};

#endif
