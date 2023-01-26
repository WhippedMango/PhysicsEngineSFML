//
//  Engine.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 19/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

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


#include <iostream>

class Engine {
    
private:
    std::vector<Ball> balls;
    std::vector<Block> blocks;
    std::vector<Ball*> ballPool; // pool of Ball objects
    //std::vector<sf::CircleShape> shapes;
    std::vector<sf::RectangleShape> shapes;
    PhysicsEngine physics;
    Quadtree* quadtree;
    int maxObjects;
    float bounce = 50; // Height of bounce
    
public:
    Engine(sf::RenderWindow &window) {
        // Initialise the physics engine
        physics = PhysicsEngine();
        // Allocate size of the balls vector
        balls.reserve(25);
        blocks.reserve(25);
        
        // Allocate size of the pool vector
//        ballPool.reserve(25);
        
        // Get the window size
        float windowx = window.getSize().x;
        float windowy = window.getSize().y;
        
        // Fill the pool with Ball objects
//        for (int i = 0; i < ballPool.capacity(); i++) {
//            ballPool.push_back(new Ball(0, 0, 0, 0, 0, 0, 0, 0, physics));
//        }
        maxObjects = 10;
        quadtree = new Quadtree(maxObjects, sf::FloatRect(0, 0, windowx, windowy));
    }
    
    ~Engine() {
        for (auto ball : ballPool) {
            delete ball;
        }
        balls.clear();
        blocks.clear();
//        ballPool.clear();
        shapes.clear();
        delete quadtree;
    }
    
    void run(sf::RenderWindow &window) {
        
        
        // Initialise KE value for debug
        float ke = 0;
        
        // Get the window size
        float windowx = window.getSize().x;
        float windowy = window.getSize().y;
        
        while (window.isOpen()) {
            sf::Clock clock;
            // Handle events
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed) {
                    //  KeyEventHandler::handleKeyPressed(event, balls, shapes, quadtree, window, physics);
                    if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
                        std::cout << "key press detected" << std::endl;  // Debug print statement
                        if (balls.size() < balls.capacity()) {
                            quadtree->insert(&balls.back());
                            sf::Vector2f randomPosition = getRandomPosition(window);
                            float new_x = randomPosition.x;
                            float new_y = randomPosition.y;
                            float new_vx = rand() % 20 - 10;
                            float new_vy = rand() % 20 - 10;
                            Ball new_ball;
                            Block new_block;
                            // Use the Quadtree to determine if a shape will randomly spawn on another shape and if so, move it
                            for (int i = 0; i < balls.size(); i++) {
                                std::vector<Object*> nearbyObjects = quadtree->retrieve(balls[i].getGlobalBounds());
                                for (int j = i + 1; j < nearbyObjects.size(); j++) {
                                    if (physics.checkCollision(new_ball, *nearbyObjects[j])) {
                                        new_ball.x += new_ball.radius/2;
                                        new_ball.y += new_ball.radius/2;
                                    }
                                }
                            }
                            for (int i = 0; i < blocks.size(); i++) {
                                std::vector<Object*> nearbyObjects = quadtree->retrieve(blocks[i].getGlobalBounds());
                                for (int j = i + 1; j < nearbyObjects.size(); j++) {
                                    if (physics.checkCollision(new_ball, *nearbyObjects[j])) {
                                        new_block.x += new_block.radius/2;
                                        new_block.y += new_block.radius/2;
                                    }
                                }
                            }
                            //new_ball = Ball(new_x, new_y, new_vx, new_vy, ax, ay, mass, radius, omega, inertia, physics);
                            new_block = Block(new_x, new_y, new_vx, new_vy, ax, ay, mass, width, height, omega, inertia, physics);
                            //balls.push_back(new_ball);
                            blocks.push_back(new_block);
//                            circles.push_back(ShapeFactory::createCircleShape(1, radius, sf::Vector2f(new_x, new_y))[0]);
//                            for (const auto& circle : circles) {
//                                    shapes.push_back(circle);
//                                }
                            shapes.push_back(ShapeFactory::createSquareShape(1, sf::Vector2f(new_x, new_y))[0]);
                            //shapes.push_back(ShapeFactory::createCircleShape(1, radius, sf::Vector2f(new_x, new_y))[0]);
                        }
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
//                    handleMousePressed(event, balls, shapes, window, bounce);
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        for (int i = 0; i < shapes.size(); i++) {
                            if (shapes[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                                balls[i].vx = -balls[i].vx;
//                                balls[i].ay = -bounce;
                                blocks[i].vx = -blocks[i].vx;
                                blocks[i].ay = -bounce;
                            }
                        }
                    }
                }
            }
            
            // Clear the previous frame
            window.clear();

            for (int i = 0; i < 5; i++){
                // Apply gravity & Update
//                for (int i = 0; i < balls.size(); i++) {
//                    // Add the kinetic energy
//                    ke += 0.5 * balls[i].mass * (balls[i].vx * balls[i].vx + balls[i].vy * balls[i].vy);
//
//                    balls[i].ay = GRAVITY;
//                    balls[i].update(TIMESTEP/5, windowx, windowy, 0.9);
//
//                    std::vector<Object*> nearbyObjects = quadtree->retrieve(balls[i].getGlobalBounds());
//                    // Check for collision between balls and handle
////                    for (int j = i + 1; j < nearbyObjects.size(); j++) {
////                        if (physics.checkCollision(balls[i], *nearbyObjects[j])) {
////                            physics.handleCollision(balls[i], *nearbyObjects[j], 0.9);
////                        }
////                    }
//                    for (int j = i + 1; j < balls.size(); j++) {
//                        if (physics.checkCollision(balls[i], balls[j])) {
//                            physics.handleCollision(balls[i], balls[j], 0.9);
//                        }
//                    }
//                }
                for (int i = 0; i < blocks.size(); i++) {
                    // Add the kinetic energy
                    ke += 0.5 * blocks[i].mass * (blocks[i].vx * blocks[i].vx + blocks[i].vy * blocks[i].vy);
                    
                    blocks[i].ay = GRAVITY;
                    blocks[i].update(TIMESTEP/5, windowx, windowy, 0.9);
                    
                    std::vector<Object*> nearbyObjects = quadtree->retrieve(blocks[i].getGlobalBounds());
                    // Check for collision between balls and handle
//                    for (int j = i + 1; j < nearbyObjects.size(); j++) {
//                        if (physics.checkCollision(balls[i], *nearbyObjects[j])) {
//                            physics.handleCollision(balls[i], *nearbyObjects[j], 0.9);
//                        }
//                    }
                    for (int j = i + 1; j < balls.size(); j++) {
                        if (physics.checkCollision(blocks[i], blocks[j])) {
                            physics.handleCollision(blocks[i], blocks[j], 0.9);
                        }
                    }
                }
            }

            // Update shape position and draw to screen
            for (int i = 0; i < shapes.size(); i++) {
                shapes[i].setPosition(blocks[i].x, blocks[i].y);
                window.draw(shapes[i]);
            }
            
            // Debug print statement of Kinetic Energy
            std::cout << ke << std::endl;
            
            // Display the window contents
            window.display();
            sf::Time elapsed = clock.getElapsedTime();
            float elap = elapsed.asSeconds();
            while (elap < 1/60){
                float elap = elapsed.asSeconds();
            }
            clock.restart();
        }
    }
};

#endif /* Engine_hpp */

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
