#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "ResourcePath.hpp"
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
    sf::Vector2f startPos;
    sf::Vector2f currentPos;
    float mouseDragDistance = 0;
    bool isDragging = false;
    sf::Texture texture;
    sf::Sprite sprite;

    
public:
    Engine(sf::RenderWindow &window) {
        // Initialise the physics engine
        physics = PhysicsEngine();
        
        // Set up the quadtree
        int maxObjects = 30;
        quadtree = new Quadtree(maxObjects, sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
        
        // Allocate size of the balls vector
        balls.reserve(maxObjects);
        texture.loadFromFile(resourcePath() + "/final.PNG");
        sprite = sf::Sprite(texture);
        sprite.setScale(0.10f, 0.10f);
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
                else if (event.type == sf::Event::KeyPressed) {
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
                else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
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
                                    balls[i].spring(15, mouseDragDistance, currentPos, startPos);
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
                sprite.setPosition(balls[i].x- balls[i].radius, balls[i].y- balls[i].radius);
                window.draw(shapes[i]);
                window.draw(sprite);
            }
            
            
            // Debug print statement of Kinetic Energy
            std::cout << ke << std::endl;
            ke = 0;
            // Display the window contents
            window.display();
            //
        }
    }
};

#endif


//
//  Engine.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 19/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

//#ifndef ENGINE_HPP
//#define ENGINE_HPP
//
//#include "ResourcePath.hpp"
//#include "RandomPosition.hpp"
//#include "SFMLObjects.hpp"
//#include "PhysicsEngine.hpp"
//#include "PhysicsConstants.hpp"
//#include "Ball.hpp"
//#include "Block.hpp"
//#include "ShapeFactory.hpp"
//#include "Quadtree.hpp"
//#include "KeyEventHandler.hpp"
//#include "MouseEventHandler.hpp"
//
//
//#include <iostream>
//
//class Engine {
//
//private:
//    std::vector<Block> blocks;
//    std::vector<sf::RectangleShape> shapes;
//    PhysicsEngine physics;
//    Quadtree* quadtree;
//    int maxObjects;
//    float bounce = 50; // Height of bounce
//
//public:
//    Engine(sf::RenderWindow &window) : physics() {
//        // Initialise the physics engine
//        physics = PhysicsEngine();
//        // Allocate size of the balls vector
//        blocks.reserve(25);
//
//        // Get the window size
//        float windowx = window.getSize().x;
//        float windowy = window.getSize().y;
//
//        maxObjects = 10;
//        quadtree = new Quadtree(maxObjects, sf::FloatRect(0, 0, windowx, windowy));
//    }
//
//    ~Engine() {
//        blocks.clear();
//        shapes.clear();
//        delete quadtree;
//    }
//
//    void run(sf::RenderWindow &window) {
//
//        // Get the window size
//        float windowx = window.getSize().x;
//        float windowy = window.getSize().y;
//
//        while (window.isOpen()) {
//            sf::Clock clock;
//            // Handle events
//            sf::Event event;
//            while (window.pollEvent(event)) {
//                if (event.type == sf::Event::Closed) {
//                    window.close();
//                }
//                else if (event.type == sf::Event::KeyPressed) {
//                    //  KeyEventHandler::handleKeyPressed(event, balls, shapes, quadtree, window, physics);
//                    if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
//                        std::cout << "key press detected" << std::endl;  // Debug print statement
//                        if (blocks.size() < blocks.capacity()) {
//                            sf::Vector2f randomPosition = getRandomPosition(window);
//                            float new_x = randomPosition.x;
//                            float new_y = randomPosition.y;
//                            float new_vx = rand() % 20 - 10;
//                            float new_vy = rand() % 20 - 10;
//                            // Use the Quadtree to determine if a shape will randomly spawn on another shape and if so, move it
//                            Block new_block(new_x, new_y, new_vx, new_vy, ax, ay, mass, width, height, omega, inertia, physics);
//                            blocks.push_back(new_block);
//                            shapes.push_back(ShapeFactory::createSquareShape(1, sf::Vector2f(new_x, new_y))[0]);
//                        }
//                    }
//                }
//                else if (event.type == sf::Event::MouseButtonPressed) {
//                    if (event.mouseButton.button == sf::Mouse::Left) {
//                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//                        for (int i = 0; i < blocks.size(); i++) {
//                            if (shapes[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                                physics.bounce(blocks[i], 0.9);
//                            }
//                        }
//                    }
//                }
//            }
//
//            // Clear the previous frame
//            window.clear();
//
//            for (int i = 0; i < 5; i++){
//                for (int i = 0; i < blocks.size(); i++) {
//                    blocks[i].ay = GRAVITY;
//                    blocks[i].update(TIMESTEP, windowx, windowy, 0.9);
//                    for (int j = i + 1; j < blocks.size(); j++) {
//                        blocks[i].collide(blocks[j], 0.9);
//                    }
//                }
//            }
//
//            // Update shape position and draw to screen
//            for (int i = 0; i < shapes.size(); i++) {
//                shapes[i].setPosition(blocks[i].x, blocks[i].y);
//                window.draw(shapes[i]);
//            }
//
//            // Display the window contents
//            window.display();
//            sf::Time elapsed = clock.getElapsedTime();
//            float elap = elapsed.asSeconds();
//            while (elap < 1/60){
//                float elap = elapsed.asSeconds();
//            }
//            clock.restart();
//        }
//    }
//};
//
//#endif /* Engine_hpp */



//quadtree->insert(*blocks.back());
//for (int i = 0; i < blocks.size(); i++) {
//    std::vector<Object*> nearbyObjects = quadtree->retrieve(blocks[i].getGlobalBounds());
//    for (int j = i + 1; j < nearbyObjects.size(); j++) {
//        if (physics.checkCollision(new_block, *nearbyObjects[j])) {
//            new_block.x += new_block.width/2;
//            new_block.y += new_block.width/2;
//        }
//    }
//}

// Check for collision between balls and handle
//                    for (int j = i + 1; j < nearbyObjects.size(); j++) {
//                        if (physics.checkCollision(balls[i], *nearbyObjects[j])) {
//                            physics.handleCollision(balls[i], *nearbyObjects[j], 0.9);
//                        }
//                    }


//#ifndef ENGINE_HPP
//#define ENGINE_HPP
//
//#include "ResourcePath.hpp"
//#include "RandomPosition.hpp"
//#include "SFMLObjects.hpp"
//#include "PhysicsEngine.hpp"
//#include "PhysicsConstants.hpp"
//#include "Ball.hpp"
//#include "Block.hpp"
//#include "ShapeFactory.hpp"
//#include "Quadtree.hpp"
//#include "KeyEventHandler.hpp"
//#include "MouseEventHandler.hpp"
//#include "Spring.hpp"
//
//
//#include <iostream>
//
//class Engine {
//
//private:
//    std::vector<Ball> balls;
//    std::vector<Block> blocks;
//    std::vector<Ball*> ballPool; // pool of Ball objects
//    std::vector<sf::CircleShape> shapes;
//    //std::vector<sf::RectangleShape> shapes;
//    PhysicsEngine physics;
//    Quadtree* quadtree;
//    int maxObjects;
//    float bounce = 50; // Height of bounce
//
//public:
//    Engine(sf::RenderWindow &window) {
//        // Initialise the physics engine
//        physics = PhysicsEngine();
//        // Allocate size of the balls vector
//        balls.reserve(25);
//        blocks.reserve(25);
//
//        // Allocate size of the pool vector
////        ballPool.reserve(25);
//        // Fill the pool with Ball objects
////        for (int i = 0; i < ballPool.capacity(); i++) {
////            ballPool.push_back(new Ball(0, 0, 0, 0, 0, 0, 0, 0, physics));
////        }
//
//        int maxObjects = blocks.size();
//
//        // Get the window size
//        float windowx = window.getSize().x;
//        float windowy = window.getSize().y;
//
//        maxObjects = 10;
//        quadtree = new Quadtree(maxObjects, sf::FloatRect(0, 0, windowx, windowy));
//    }
//
//    ~Engine() {
////        for (auto ball : ballPool) {
////            delete ball;
////        }
//        balls.clear();
//        blocks.clear();
////        ballPool.clear();
//        shapes.clear();
//        delete quadtree;
//    }
//
//    void run(sf::RenderWindow &window) {
//
//
//        // Initialise KE value for debug
//        float ke = 0;
//
//        // Get the window size
//        float windowx = window.getSize().x;
//        float windowy = window.getSize().y;
//
//        while (window.isOpen()) {
//            sf::Clock clock;
//            // Handle events
//            sf::Event event;
//            while (window.pollEvent(event)) {
//                if (event.type == sf::Event::Closed) {
//                    window.close();
//                }
//                else if (event.type == sf::Event::KeyPressed) {
//                    //  KeyEventHandler::handleKeyPressed(event, balls, shapes, quadtree, window, physics);
//                    if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
//                        std::cout << "key press detected" << std::endl;  // Debug print statement
//                        if (blocks.size() < blocks.capacity()) {
//
//                            quadtree->insert(&blocks.back());
//                            quadtree->insert(&balls.back());
//                            sf::Vector2f randomPosition = getRandomPosition(window);
//                            float new_x = randomPosition.x;
//                            float new_y = randomPosition.y;
//                            float new_vx = rand() % 20 - 10;
//                            float new_vy = rand() % 20 - 10;
//                            Block new_block(new_x, new_y, new_vx, new_vy, ax, ay, mass, width, height, omega, inertia, physics);
//                            Ball new_ball(new_x, new_y, new_vx, new_vy, ax, ay, mass, radius, omega, inertia, physics);
//
//                            // Use the Quadtree to determine if a shape will randomly spawn on another shape and if so, move it
//                            for (int i = 0; i < balls.size(); i++) {
//                                std::vector<Object*> nearbyObjects = quadtree->retrieve(balls[i].getGlobalBounds());
//                                for (int j = i + 1; j < nearbyObjects.size(); j++) {
//                                    balls[i].ballHere(balls[j], new_ball, window);
//                                }
//                            }
////                            for (int i = 0; i < blocks.size(); i++) {
////                                std::vector<Object*> nearbyObjects = quadtree->retrieve(blocks[i].getGlobalBounds());
////                                for (int j = i + 1; j < nearbyObjects.size(); j++) {
////                                    if (physics.checkCollision(new_ball, *nearbyObjects[j])) {
////                                        blocks[i].blockHere(blocks[j], new_block, window);
////                                    }
////                                }
//                           // }
//                            blocks.push_back(new_block);
//                            balls.push_back(new_ball);
////                          circles.push_back(ShapeFactory::createCircleShape(1, radius, sf::Vector2f(new_x, new_y))[0]);
//                            //shapes.push_back(ShapeFactory::createSquareShape(1, sf::Vector2f(new_x, new_y))[0]);
//                            shapes.push_back(ShapeFactory::createCircleShape(1, radius, sf::Vector2f(new_x, new_y))[0]);
//                        }
//                    }
//                }
//                else if (event.type == sf::Event::MouseButtonPressed) {
//                    //MouseEventHandler::handleMousePressed(event, balls, shapes, window, bounce);
//                    if (event.mouseButton.button == sf::Mouse::Left) {
//                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//                        for (int i = 0; i < shapes.size(); i++) {
//                            if (shapes[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                                balls[i].bounce(0.9);
//                            }
//                        }
//                    }
//                }
//            }
//
//            // Clear the previous frame
//            window.clear();
//
//            for (int i = 0; i < blocks.size(); i++) {
//                // Add the kinetic energy
//                ke += 0.5 * blocks[i].mass * (blocks[i].vx * blocks[i].vx + blocks[i].vy * blocks[i].vy);
//
//                blocks[i].ay = GRAVITY;
//                blocks[i].update(TIMESTEP/5, windowx, windowy, 0.9);
//
//                std::vector<Object*> nearbyObjects = quadtree->retrieve(blocks[i].getGlobalBounds());
//                // Check for collision between balls and handle
//                     for (int j = i + 1; j < nearbyObjects.size(); j++) {
//                         if (physics.checkCollision(blocks[i], *nearbyObjects[j])) {
//                             physics.handleCollision(blocks[i], *nearbyObjects[j], 0.9);
//                         }
//                     }
//            }
//            for (int i = 0; i < balls.size(); i++) {
//                balls[i].ay = GRAVITY;
//                balls[i].update(TIMESTEP/5, windowx, windowy, 0.9);
//
//                std::vector<Object*> nearbyObjects = quadtree->retrieve(balls[i].getGlobalBounds());
//                // Check for collision between balls and handle
//                     for (int j = i + 1; j < balls.size(); j++) {
//                         if (physics.checkCollision(balls[i], balls[j])) {
//                             physics.handleCollision(balls[i], balls[j], 0.9);
//                         }
//                     }
//            }
//////                    }
////                }
////            }
//
//            // Update shape position and draw to screen
//            for (int i = 0; i < shapes.size(); i++) {
//                shapes[i].setPosition(balls[i].x, balls[i].y);
//                shapes[i].getRotation();
//                window.draw(shapes[i]);
//            }
//
//            // Debug print statement of Kinetic Energy
//            //std::cout << ke << std::endl;
//
//            // Display the window contents
//            window.display();
////
//        }
//    }
//};
//
//#endif

//for (int i = 0; i < 5; i++){
    // Apply gravity & Update
//                for (int i = 0; i < balls.size(); i++) {
//                    // Add the kinetic energy
//                    ke += 0.5 * balls[i].mass * (balls[i].vx * balls[i].vx + balls[i].vy * balls[i].vy);
//
//                    balls[i].ay = GRAVITY;
//                    balls[i].update(TIMESTEP, windowx, windowy, 0.6);
//
//                     // Check for collision between balls and handle
////                    for (int j = i + 1; j < balls.size(); j++) {
////                        if (physics.checkCollision(balls[i], *nearbyObjects[j])) {
////                            physics.handleCollision(balls[i], *nearbyObjects[j], 0.9);
////                        }
////
//////                        std::vector<Object*> nearbyObjects = quadtree->retrieve(balls[i].getGlobalBounds());
//////                        if (nearbyObjects.size() > 0)
//////                        {
//////                            balls[i].collide(*nearbyObjects[j], 0.9);
//////                        }
////                    }
//
//                    for (int j = i + 1; j < balls.size(); j++) {
//                        if (physics.checkCollision(balls[i], balls[j])) {
//                            physics.handleCollision(balls[i], balls[j], 0.99);
//                        }
//                        balls[i].collide(balls[j], 0.9);
//                    }
//                }
//}

//                            if (physics.checkCollision(balls[i], *nearbyObjects[j])) {
//                                physics.handleCollision(balls[i], *nearbyObjects[j], 0.9);
//                            }
                        //balls[i].collide(*nearbyObjects[j], 0.9);
//                        if (physics.checkCollision(balls[i], balls[j])) {
//                            physics.handleCollision(balls[i], balls[j], 0.9);
//                        }
//                        if (physics.checkCollision(balls[i], *nearbyObjects[j])) {
//                            physics.handleCollision(balls[i], *nearbyObjects[j], 0.9);
//                        }
//                        if (balls[i].collided(balls[j])){
//                          balls[i].collide(balls[j], 0.9);
//                        }

//                    if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
//                        std::cout << "key press detected" << std::endl;  // Debug print statement
//                        if (balls.size() < balls.capacity()) {
//                            quadtree->insert(&balls.back());
//                            sf::Vector2f randomPosition = getRandomPosition(window);
//                            float new_x = randomPosition.x;
//                            float new_y = randomPosition.y;
//                            float new_vx = rand() % 20 - 10;
//                            float new_vy = rand() % 20 - 10;
//                            Ball new_ball;
//                            for (int i = 0; i < balls.size(); i++) {
//                                std::vector<Object*> nearbyObjects = quadtree->retrieve(shapes[i].getGlobalBounds());
//                                for (int j = i + 1; j < nearbyObjects.size(); j++) {
//                                    if (physics.checkCollision(new_ball, *nearbyObjects[j])) {
//                                        new_ball.x += new_ball.radius/2;
//                                    }
//                                }
//                            }
//                            new_ball = Ball(new_x, new_y, new_vx, new_vy, ax, ay, mass, radius, physics);
//                            balls.push_back(new_ball);
//
//                            shapes.push_back(ShapeFactory::createCircleShape(1, radius, sf::Vector2f(new_x, new_y))[0]);
//                        }
//                    }
//                }


// // Previous collision check
//                for (int j = i + 1; j < balls.size(); j++) {
//                    // Check for collision between balls
//                    if (physics.checkCollision(balls[i], balls[j])) {
//                        // Handle collision
//                        physics.handleCollision(balls[i], balls[j]);
//                    }
//                }

// // Handle collisions
//std::vector<Object*> nearbyObjects = quadtree->retrieve(shapes[i].getGlobalBounds());
//for (int j = i + 1; j < nearbyObjects.size(); j++) {
//    if (physics.checkCollision(balls[i], *nearbyObjects[j])) {
//        physics.handleCollision(balls[i], *nearbyObjects[j]);
//    }
//}

//    float x = rand() % 500;
//    float y = rand() % 500;
//    float vx = rand() % 20 - 10; // Random velocity between -10 and 10
//    float vy = rand() % 20 - 10;
//    float ax = 0;
//    float ay = 0;
//    float mass = 0.4; // 1 kg
//    float radius = 100;

//for (int j = i + 1; j < balls.size(); j++) {
//    // Check for collision between balls
//    if (physics.checkCollision(balls[i], balls[j])) {
//        // Handle collision
//        physics.handleCollision(balls[i], balls[j]);
//    }
//}

// Create the ball object to be rendered
//shapes.push_back(ShapeFactory::createCircleShape(5, radius, sf::Color::Red, sf::Vector2f(ball.x, ball.y)));
//shape = ShapeFactory::createCircleShape(5, radius, sf::Color::Red, sf::Vector2f(x, y));


// shapes = ShapeFactory::createCircleShape(ball.x, ball.y, radius, sf::Color::Red);
// shape = ShapeFactory::createCircleShape(radius, sf::Color::Red, sf::Vector2f(ball.x, ball.y));

// Draw the balls
//for (auto &shape : shapes) {
//    window.draw(shape);
//}

//            for (int i = 0; i < balls.size(); i++) {
//                balls[i].ay = GRAVITY;
//                balls[i].update(TIMESTEP, window.getSize().x, window.getSize().y);
//                shapes[i].setPosition(balls[i].x, balls[i].y);
//                window.draw(shapes[i]);
//            }

//                            shapes[i].setOrigin(radius/2, radius/2);
//                            shapes[i].scale(1, 1);
//                            shapes[i].setPosition(balls[i].x, balls[i].y);
//                            window.draw(shapes[i]);


// Apply gravity and update positions for each ball
//            for (auto &ball : balls) {
//                ball.ay = GRAVITY;
//                ball.update(TIMESTEP, window.getSize().x, window.getSize().y);
//            }

// Load music to play
//        sf::Music music;
//        if (!music.openFromFile(resourcePath() + "nice_music.wav")) {
//            return EXIT_FAILURE;
//        }
//        float pitch = 1.05;
//        music.play();
//        music.setLoop(1);
//        music.setVolume(0.45);
//                        music.setPitch(pitch);
//                        pitch += (rand() % 25) - 115;
//
//    // Load a sprite to display
//    texture.loadFromFile(resourcePath() + "cute_image.gif");
//    sprite = sf::Sprite(texture);
//    sprite.setScale(0.14f, 0.14f);

//            ball.ay = GRAVITY;
//            ball.update(TIMESTEP, window.getSize().x, window.getSize().y);
//
//            // Set the position of the sprite/ball to be rendered
//            sprite.setPosition(ball.x, ball.y);
//            sprite.setOrigin(radius/2, radius/2);
//            shape = ShapeFactory::createCircleShape(1, radius, sf::Color::Blue, sf::Vector2f(ball.x, ball.y))[0];
//            shape.setPosition(ball.x, ball.y);
//
//            // Draw the shapes
//            window.draw(shape);
//            window.draw(sprite);

//Ball ball;
//sf::CircleShape shape;
//sf::Texture texture;
//sf::Sprite sprite;
