#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "../SceneManager/SceneManager.h"
#include "Scene.h"

class GameOverScene : public Scene {
private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text restartText;
    sf::Text mainMenuText;

    SceneManager* sceneManager;

    void initializeText() {
        // Load font
        if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }

        // Setup Game Over Text
        gameOverText.setFont(font);
        gameOverText.setString("Game Over");
        gameOverText.setCharacterSize(48);
        gameOverText.setFillColor(sf::Color::Red);
        sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
        gameOverText.setPosition(
            (800 - gameOverBounds.width) / 2 - gameOverBounds.left,
            150);

        // Setup Restart Text
        restartText.setFont(font);
        restartText.setString("Press R to Restart");
        restartText.setCharacterSize(24);
        restartText.setFillColor(sf::Color::White);
        sf::FloatRect restartBounds = restartText.getLocalBounds();
        restartText.setPosition(
            (800 - restartBounds.width) / 2 - restartBounds.left,
            300); // Adjust the position as needed

        // Setup Main Menu Text
        mainMenuText.setFont(font);
        mainMenuText.setString("Press ESC to Main Menu");
        mainMenuText.setCharacterSize(24);
        mainMenuText.setFillColor(sf::Color::White);
        sf::FloatRect mainMenuBounds = mainMenuText.getLocalBounds();
        mainMenuText.setPosition(
            (800 - mainMenuBounds.width) / 2 - mainMenuBounds.left,
            350); // Adjust the position as needed
    }

public:
    GameOverScene(SceneManager* sceneMgr) : sceneManager(sceneMgr) {
        initializeText();
    }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    // Return to main menu
                    if (sceneManager) {
                        sceneManager->setActiveScene("MainMenu");
                        sceneManager->removeScene("InGame");
                    }
                }
                else if (event.key.code == sf::Keyboard::R) {
                    // Restart the game scene
                    if (sceneManager) {
                        // Remove and recreate InGameScene
                        sceneManager->removeScene("InGame");

                        // Create new InGameScene
                        auto entityManager = std::make_shared<EntityManager>();
                        auto inGameScene = std::make_shared<InGameScene>(entityManager, sceneManager);
                        sceneManager->addScene("InGame", inGameScene);
                        sceneManager->setActiveScene("InGame");
                        inGameScene->onActivate();
                        inGameScene->spawnMonsters();
                    }
                }
            }
        }
    }

    void update(float dt) override {}

    void render(sf::RenderWindow& window) override {
        window.draw(gameOverText);
        gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getLocalBounds().height / 2);
        gameOverText.setPosition(window.getSize().x / 2, 150);
        window.draw(restartText);
        restartText.setOrigin(restartText.getLocalBounds().width / 2, restartText.getLocalBounds().height / 2);
        restartText.setPosition(window.getSize().x / 2, 300);
        window.draw(mainMenuText);
        mainMenuText.setOrigin(mainMenuText.getLocalBounds().width / 2, mainMenuText.getLocalBounds().height / 2);
        mainMenuText.setPosition(window.getSize().x / 2, 350);
    }

    void onActivate() override {}
};
