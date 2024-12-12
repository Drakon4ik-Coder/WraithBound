#pragma once
#include <SFML/Graphics.hpp>
#include "../SceneManager/SceneManager.h"
#include "Scene.h"

class VictoryScene : public Scene {
private:
    sf::Font font;
    sf::Text victoryText;
    SceneManager* sceneManager;

public:
    VictoryScene(SceneManager* sceneMgr) : sceneManager(sceneMgr) {
        if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }

        victoryText.setFont(font);
        victoryText.setString("Victory!\nYou have defeated all monsters!\nPress Enter to return to the main menu.");
        victoryText.setCharacterSize(24);
        victoryText.setFillColor(sf::Color::Green);
        victoryText.setOrigin(victoryText.getLocalBounds().width / 2, victoryText.getLocalBounds().height / 2);
        victoryText.setPosition(window.get()->getSize().x/2, 300);  // Adjust position as needed
    }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    sceneManager->setActiveScene("MainMenu");
                }
            }
        }
    }

    void update(float dt) override {}

    void render(sf::RenderWindow& window) override {
        window.draw(victoryText);
    }

    void onActivate() override {}
};
