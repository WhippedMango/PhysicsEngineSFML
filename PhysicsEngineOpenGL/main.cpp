#include "Engine.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1800, 1600), "Physics Engine");
    Engine engine(window);
    engine.run(window);
    return 0;
}

//    Object ball;
//    ball.x = (window.getSize().x)/2;
//    ball.y = (window.getSize().y)/2;
//    ball.ax = 0;
//    ball.ay = 0;
//    ball.vx = 0;
//    ball.vy = -10;
//    ball.mass = 100; // 1 kg


// Get the current position of the mouse cursor
//sf::Vector2i mousePos = sf::Mouse::getPosition(window);
// Set the position of the ball to be rendered (mouse)
// shape.setPosition(mousePos.x, mousePos.y);


//#include <SFML/Audio.hpp>
//#include <SFML/Graphics.hpp>
//#include <SFML/System.hpp>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//
//#include "ResourcePath.hpp"
//
//int main(int, char const**)
//{
//    // Create the main window
//    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
//
//    // Set the Icon
//    sf::Image icon;
//    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
//        return EXIT_FAILURE;
//    }
//    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
//
//    // Load a sprite to display
//    sf::Texture texture;
//    if (!texture.loadFromFile(resourcePath() + "cute_image.gif")) {
//        return EXIT_FAILURE;
//    }
//    sf::Sprite sprite(texture);
//
//    // Create a graphical text to display
//    sf::Font font;
//    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
//        return EXIT_FAILURE;
//    }
//    sf::Text text("Hello SFML", font, 50);
//    text.setFillColor(sf::Color::Black);
//    text.setPosition(400, 300);
//
//    // Load a music to play
//    sf::Music music;
//    if (!music.openFromFile(resourcePath() + "nice_music.wav")) {
//        return EXIT_FAILURE;
//    }
//
//    // Play the music
//    music.play();
//    music.setVolume(0.95);
//
//    // Start the game loop
//    while (window.isOpen())
//    {
//        // Process events
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            // Close window: exit
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//
//            // Escape pressed: exit
//            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
//                window.close();
//            }
//        }
//
//        // Clear screen
//        window.clear();
//
//        // Draw the sprite
//        window.draw(sprite);
//
//        // Draw the string
//        window.draw(text);
//
//        // Update the window
//        window.display();
//    }
//
//    return EXIT_SUCCESS;
//}
