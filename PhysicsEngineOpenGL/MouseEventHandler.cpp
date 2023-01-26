//
//  MouseEventHandler.cpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 24/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#include "MouseEventHandler.hpp"

void handleMousePressed(sf::Event event, std::vector<Ball> &balls, std::vector<sf::CircleShape> &shapes, sf::RenderWindow &window, float bounce) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            for (int i = 0; i < shapes.size(); i++) {
                if (shapes[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    balls[i].vx = -balls[i].vx;
                    balls[i].vy += bounce;
                }
            }
        }
    }
}
