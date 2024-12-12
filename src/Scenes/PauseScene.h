// ../WraithBound/src/Scenes/PauseScene.h
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "../SceneManager/SceneManager.h"
#include "Scene.h"

class PauseScene : public Scene {
private:
    sf::Font font;
    sf::RectangleShape resumeButton;
    sf::RectangleShape quitButton;

    sf::Text resumeText;
    sf::Text quitText;

    SceneManager* sceneManager;  // Pointer to SceneManager

    void initializeButtons() {
        // Load font
        if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }

        // Setup Resume Button
        resumeButton.setSize(sf::Vector2f(200, 50));
        resumeButton.setFillColor(sf::Color::Green);
        resumeButton.setPosition(300, 200);

        resumeText.setFont(font);
        resumeText.setString("Resume");
        resumeText.setCharacterSize(24);
        resumeText.setFillColor(sf::Color::White);
        sf::FloatRect resumeBounds = resumeText.getLocalBounds();
        resumeText.setPosition(
            resumeButton.getPosition().x +
            (resumeButton.getSize().x - resumeBounds.width) / 2 -
            resumeBounds.left,
            resumeButton.getPosition().y +
            (resumeButton.getSize().y - resumeBounds.height) / 2 -
            resumeBounds.top);

        // Setup Quit Button
        quitButton.setSize(sf::Vector2f(200, 50));
        quitButton.setFillColor(sf::Color::Red);
        quitButton.setPosition(300, 300);

        quitText.setFont(font);
        quitText.setString("Quit to Menu");
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
    // Constructor accepts SceneManager pointer
    PauseScene(SceneManager* sceneMgr) : sceneManager(sceneMgr) {
        initializeButtons();
    }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (sceneManager) {
                        //sceneManager->removeScene("Pause");
                        // Assuming the previous scene is the game scene
                        // Adjust as per your SceneManager's implementation
                        sceneManager->setActiveScene("InGame");
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
                        static_cast<float>(mousePos.y));

                    if (resumeButton.getGlobalBounds().contains(mousePosF)) {
                        std::cout << "Resume clicked" << std::endl;
                        if (sceneManager) {
                            //sceneManager->removeScene("Pause");
                            sceneManager->setActiveScene("InGame");
                        }
                    }

                    if (quitButton.getGlobalBounds().contains(mousePosF)) {
                        std::cout << "Quit to Menu clicked" << std::endl;
                        if (sceneManager) {
                            sceneManager->setActiveScene("MainMenu");
                        }
                    }
                }
            }
        }
    }

    void update(float dt) override {
        // Pause scene might not need to update dynamic content
    }

    void render(sf::RenderWindow& window) override {
        // Optionally, draw a semi-transparent overlay
        sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        // Draw buttons and texts
        window.draw(resumeButton);
        window.draw(resumeText);
        window.draw(quitButton);
        window.draw(quitText);
    }

    void onActivate() override {
        // Any activation logic if needed
    }
};
