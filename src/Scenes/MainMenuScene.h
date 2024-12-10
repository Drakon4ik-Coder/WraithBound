#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include "Scene.h"
#include "../src/SceneManager/SceneManager.h"

// Button Class to Handle Button Logic
class Button {
public:
    Button(const std::string& text, const sf::Vector2f& position) {
        buttonText.setString(text);
        buttonText.setCharacterSize(30);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(position);

        // Button background
        buttonShape.setSize(sf::Vector2f(200, 50));
        buttonShape.setFillColor(sf::Color::Blue);
        buttonShape.setPosition(position.x - 10, position.y - 10);
    }

    bool isClicked(const sf::Vector2i& mousePos) {
        return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

    void render(sf::RenderWindow& window) {
        window.draw(buttonShape);
        window.draw(buttonText);
    }

    sf::Text buttonText;
    sf::RectangleShape buttonShape;
};

// Main Menu Scene Class
class MainMenuScene : public Scene {
private:
    sf::Font font;
    sf::Text titleText;
    //sf::Texture backgroundTexture;
    //sf::Sprite background;
    std::vector<Button> buttons;
    std::shared_ptr<SceneManager> _sceneManager;

public:
    MainMenuScene() {
        // Load font for text rendering
        if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
        }

        // Title text
        titleText.setFont(font);
        titleText.setString("Main Menu");
        titleText.setCharacterSize(35);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(300, 50);

        // Create buttons
        buttons.push_back(Button("Play", sf::Vector2f(300, 250)));
        buttons.push_back(Button("Settings", sf::Vector2f(300, 320)));
        buttons.push_back(Button("Exit", sf::Vector2f(300, 390)));
    }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Check if any button is clicked
                if (buttons[0].isClicked(mousePos)) {
                    std::cout << "Play Button Clicked!" << std::endl;

                    // Transition to the game (InGameScene)
                    window.clear();

                    // Assume that "InGame" scene is set as active here
                    _sceneManager->setActiveScene("InGame");
                }
                else if (buttons[1].isClicked(mousePos)) {
                    std::cout << "Settings Button Clicked!" << std::endl;
                    // Transition to Settings Scene
                }
                else if (buttons[2].isClicked(mousePos)) {
                    std::cout << "Exit Button Clicked!" << std::endl;
                    window.close();
                }
            }
        }
    }

    void update(float dt) override {
        // Any additional logic for updating the main menu (e.g., animations, etc.)
    }

    void render(sf::RenderWindow& window) override {
        // Reset to the default view for the menu
        window.setView(window.getDefaultView()); // Ensure we're using the default view for the menu
        window.clear();
        //window.draw(background);  // Draw background
        window.draw(titleText);    // Draw title text

        // Draw buttons
        for (auto& button : buttons) {
            button.render(window);
        }
    }


    // Implement getPlayer method
    std::shared_ptr<Player> getPlayer() const override {
        // Since there is no player in the main menu, return nullptr.
        return nullptr;
    }
};
