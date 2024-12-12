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
        victoryText.setString("Victory! You have defeated all monsters!");
        victoryText.setCharacterSize(36);
        victoryText.setFillColor(sf::Color::White);
        victoryText.setPosition(100, 300);  // Adjust position as needed
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

    void update(float dt) override {
        // No dynamic content to update in the victory screen
    }

    void render(sf::RenderWindow& window) override {
        window.draw(victoryText);
    }

    void onActivate() override {
        // Any activation logic if needed
    }
};
