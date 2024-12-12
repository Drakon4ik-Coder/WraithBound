#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "../EntityManager.h"
#include "../SceneManager/SceneManager.h"
#include "InGameScene.h"
#include "Scene.h"

class MainMenuScene : public Scene {
   private:
    sf::Font font;
    sf::RectangleShape startButton;
    sf::RectangleShape settingsButton;
    sf::RectangleShape quitButton;

    sf::Text startText;
    sf::Text settingsText;
    sf::Text quitText;

    SceneManager* sceneManager;
    std::shared_ptr<EntityManager> entityManager;

    void initializeButtons() {
        // Load font
        if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }

        // Setup Start Button
        startButton.setSize(sf::Vector2f(200, 50));
        startButton.setFillColor(sf::Color::Blue);
        startButton.setPosition(300, 200);

        startText.setFont(font);
        startText.setString("Start Game");
        startText.setCharacterSize(24);
        startText.setFillColor(sf::Color::White);
        // Center the text within the button
        sf::FloatRect startBounds = startText.getLocalBounds();
        startText.setPosition(
            startButton.getPosition().x +
                (startButton.getSize().x - startBounds.width) / 2 -
                startBounds.left,
            startButton.getPosition().y +
                (startButton.getSize().y - startBounds.height) / 2 -
                startBounds.top);

        // Setup Settings Button
        settingsButton.setSize(sf::Vector2f(200, 50));
        settingsButton.setFillColor(sf::Color::Green);
        settingsButton.setPosition(300, 300);

        settingsText.setFont(font);
        settingsText.setString("Settings");
        settingsText.setCharacterSize(24);
        settingsText.setFillColor(sf::Color::White);
        sf::FloatRect settingsBounds = settingsText.getLocalBounds();
        settingsText.setPosition(
            settingsButton.getPosition().x +
                (settingsButton.getSize().x - settingsBounds.width) / 2 -
                settingsBounds.left,
            settingsButton.getPosition().y +
                (settingsButton.getSize().y - settingsBounds.height) / 2 -
                settingsBounds.top);

        // Setup Quit Button
        quitButton.setSize(sf::Vector2f(200, 50));
        quitButton.setFillColor(sf::Color::Red);
        quitButton.setPosition(300, 400);

        quitText.setFont(font);
        quitText.setString("Quit");
        quitText.setCharacterSize(24);
        quitText.setFillColor(sf::Color::White);
        sf::FloatRect quitBounds = quitText.getLocalBounds();
        quitText.setPosition(
            quitButton.getPosition().x +
                (quitButton.getSize().x - quitBounds.width) / 2 -
                quitBounds.left,
            quitButton.getPosition().y +
                (quitButton.getSize().y - quitBounds.height) / 2 -
                quitBounds.top);
    }

   public:
    MainMenuScene(SceneManager* sceneMgr) : sceneManager(sceneMgr) {
        initializeButtons();
    }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
                        static_cast<float>(mousePos.y));

                    if (startButton.getGlobalBounds().contains(mousePosF)) {
                        std::cout << "Start Game clicked" << std::endl;
                        if (sceneManager) {
                            // Check if "InGame" scene already exists
                            auto inGameScene = std::dynamic_pointer_cast<InGameScene>(
                                sceneManager->getScene("InGame"));
                            if (inGameScene) {
                                // Scene exists, set it as active
                                sceneManager->setActiveScene("InGame");
                            }
                            else {
                                // Create new InGameScene
                                entityManager = std::make_shared<EntityManager>();
                                inGameScene = std::make_shared<InGameScene>(entityManager, sceneManager);
                                sceneManager->addScene("InGame", inGameScene);
                                sceneManager->setActiveScene("InGame");
                                inGameScene->onActivate();  
                                inGameScene->spawnMonsters(); 
                            }
                        }
                        else {
                            std::cerr << "SceneManager is null!" << std::endl;
                        }
                    }

                    if (settingsButton.getGlobalBounds().contains(mousePosF)) {
                        std::cout << "Settings clicked" << std::endl;
                        // Implement settings functionality here
                        sceneManager->setActiveScene("Settings");
                    }

                    if (quitButton.getGlobalBounds().contains(mousePosF)) {
                        std::cout << "Quit clicked, closing window."
                            << std::endl;
                        window.close();
                    }

                }
            }
        }
    }

    void update(float dt) override {}

    void render(sf::RenderWindow& window) override {
        window.draw(startButton);
        startButton.setOrigin(startButton.getLocalBounds().width / 2, startButton.getLocalBounds().height / 2);
        startButton.setPosition(window.getSize().x / 2, 200);
        window.draw(startText);
        startText.setOrigin(startText.getLocalBounds().width / 2, startText.getLocalBounds().height / 2);
        startText.setPosition(window.getSize().x / 2, 200);
        window.draw(settingsButton);
        settingsButton.setOrigin(settingsButton.getLocalBounds().width / 2, settingsButton.getLocalBounds().height / 2);
        settingsButton.setPosition(window.getSize().x / 2, 300);
        window.draw(settingsText);
        settingsText.setOrigin(settingsText.getLocalBounds().width / 2, settingsText.getLocalBounds().height / 2);
        settingsText.setPosition(window.getSize().x / 2, 300);
        window.draw(quitButton);
        quitButton.setOrigin(quitButton.getLocalBounds().width / 2, quitButton.getLocalBounds().height / 2);
        quitButton.setPosition(window.getSize().x / 2, 400);
        window.draw(quitText);
        quitText.setOrigin(quitText.getLocalBounds().width / 2, quitText.getLocalBounds().height / 2);
        quitText.setPosition(window.getSize().x / 2, 400);
    }

    void onActivate() override {}
};
