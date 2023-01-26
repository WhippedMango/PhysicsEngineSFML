//
//  MouseEventHandler.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 24/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#ifndef MOUSEEVENTHANDLER_hpp
#define MOUSEEVENTHANDLER_hpp

#include "Engine.hpp"
#include "PhysicsEngine.hpp"
#include "SFMLObjects.hpp"
#include <stdio.h>

void handleMousePressed(sf::Event event, std::vector<Ball*> &balls, std::vector<sf::CircleShape> &shapes, sf::RenderWindow &window);

#endif /* MouseEventHandler_hpp */
