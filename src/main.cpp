#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int main(){

    const char* display = std::getenv("DISPLAY");
    if (display == nullptr) {
        std::cout << "Running in headless mode, skipping window creation." << std::endl;
    }
    else {
        sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            window.clear();
            window.draw(shape);
            window.display();
        }
    }
  
  return 0;
}