//
//  KeyEventHandler.cpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 24/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#include "KeyEventHandler.hpp"

void handleKeyPressed(sf::Event event, std::vector<Ball>& balls, std::vector<sf::CircleShape>& shapes, Quadtree* quadtree, sf::RenderWindow &window, PhysicsEngine physics);
