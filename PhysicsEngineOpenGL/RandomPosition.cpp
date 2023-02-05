//
//  RandomPosition.cpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 26/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#include "RandomPosition.hpp"

sf::Vector2f getRandomPosition(sf::RenderWindow &window) {
    float x = (rand() % window.getSize().x);
    float y = (rand() % window.getSize().y / 4 * 3);
    return sf::Vector2f(x, y);
}
