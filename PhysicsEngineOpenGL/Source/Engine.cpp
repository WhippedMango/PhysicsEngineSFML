#include "Engine.hpp"

Engine::Engine(sf::RenderWindow &window) {
    // Initialise the physics engine
    physics = PhysicsEngine();

    // Set up the quadtree
    maxObjects = 300;
    quadtree = new Quadtree(maxObjects, sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));

    // Allocate size of the balls vector
    balls.reserve(maxObjects);
    bounce = 50; // Height of bounce
}

Engine::~Engine() {
    balls.clear();
    shapes.clear();
    delete quadtree;
}

void Engine::run(sf::RenderWindow &window) {
    // Initialise KE value for debug
    float ke = 0;

    while (window.isOpen()) {
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
            float ballKE = 0.5 * balls[i].mass * (balls[i].vx * balls[i].vx + balls[i].vy * balls[i].vy);
            ke += ballKE;
            // Update ball position
            balls[i].update(TIMESTEP / 4, window.getSize().x, window.getSize().y, 0.9f);

            std::vector<Object*> nearbyObjects = quadtree->retrieve(balls[i].getGlobalBounds());
            // Check for collision between balls and handle
            for (int j = i + 1; j < balls.size(); j++) {
                if (physics.checkCollision(balls[i], balls[j])) {
                    physics.handleCollision(balls[i], balls[j], 0.8f);
                }
            }

            shapes[i].setPosition(balls[i].x - balls[i].radius / 2, balls[i].y - balls[i].radius / 2);
            window.draw(shapes[i]);
        }

        // Debug print statement of Kinetic Energy
        std::cout << ke << std::endl;
        ke = 0;

        // Display the window contents
        window.display();
    }
}
