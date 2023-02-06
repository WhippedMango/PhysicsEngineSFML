#include "Engine.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1800, 1600), "Physics Engine");
    Engine engine(window);
    engine.run(window);
    return EXIT_SUCCESS;
}
