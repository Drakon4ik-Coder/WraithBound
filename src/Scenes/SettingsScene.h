#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <string>
#include "../SceneManager/SceneManager.h" 
#include "../Scenes/MainMenuScene.h"
#include "Scene.h"


#include <filesystem>


class SettingsScene : public Scene {
public:
    SettingsScene(SceneManager* sceneManager);  // Constructor with no arguments
    void onActivate() override;
    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    //SceneManager* sceneManager;
    SceneManager* sceneManager;

private:
    std::vector<sf::Vector2u> resolutions;
    size_t currentResolutionIndex;
    void saveSettings();
    void loadSettings();
    SettingsScene() = delete;
};


SettingsScene::SettingsScene(SceneManager* sceneManager) {
    this->sceneManager = sceneManager;
    loadSettings();
    resolutions = { {800, 600}, {1024, 768}, {1280, 720}, {1920, 1080} };
    currentResolutionIndex = 0;
}

void SettingsScene::onActivate() {
    loadSettings();
}

void SettingsScene::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                currentResolutionIndex = (currentResolutionIndex + 1) % resolutions.size();
            }
            else if (event.key.code == sf::Keyboard::Down) {
                currentResolutionIndex = (currentResolutionIndex - 1 + resolutions.size()) % resolutions.size();
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                saveSettings();
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                saveSettings();
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                saveSettings();
            }
            else if (event.key.code == sf::Keyboard::Escape) {
                if (sceneManager) {
                    //std::shared_ptr<MainMenuScene> mainMenuScene = std::make_shared<MainMenuScene>(sceneManager);
                    sceneManager->setActiveScene("MainMenu");
                }
                else {
                    std::cerr << "SceneManager is null!" << std::endl;
                }
            }
        }
    }
}

void SettingsScene::update(float dt) {
    // Update logic if necessary
}

void SettingsScene::render(sf::RenderWindow& window) {
    //window.clear();
    sf::Font font;
    font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");

    sf::Text resolutionText;
    resolutionText.setFont(font);
    resolutionText.setString("Resolution: " + std::to_string(resolutions[currentResolutionIndex].x) + "x" + std::to_string(resolutions[currentResolutionIndex].y));
    resolutionText.setCharacterSize(24);
    resolutionText.setFillColor(sf::Color::White);
    resolutionText.setPosition(100, 100);

    window.draw(resolutionText);
}

void SettingsScene::saveSettings() {
    //std::string path = "res/settings/settings.txt";

    // Open file
    std::ofstream settingsFile("res/settings/settings.txt");
    if (!settingsFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << "res/settings/settings.txt"
            << "\nError: " << std::strerror(errno) << std::endl;
        return;
    }

    // Debug resolution data
    std::cout << "Writing resolution: "
        << resolutions[currentResolutionIndex].x << " x "
        << resolutions[currentResolutionIndex].y << std::endl;

    // Write data
    settingsFile << resolutions[currentResolutionIndex].x << " "
        << resolutions[currentResolutionIndex].y << std::endl;

    // Flush and close
    settingsFile.flush();
    settingsFile.close();

    if (settingsFile) {
        std::cout << "Settings saved successfully to " << "res/settings/settings.txt" << std::endl;
    }
    else {
        std::cerr << "Error occurred during writing to file: " << "res/settings/settings.txt" << std::endl;
    }
}

void SettingsScene::loadSettings() {
    //std::string path = "res/settings/settings.txt";

    std::ifstream settingsFile("res/settings/settings.txt");

    if (!settingsFile.is_open()) {
        std::cerr << "Failed to open " << "res/settings/settings.txt" << " for reading. Error: "
            << std::strerror(errno) << std::endl;
        return;
    }

    unsigned int width = 0, height = 0;
    if (!(settingsFile >> width >> height)) {
        std::cerr << "Invalid data format in " << "res/settings/settings.txt" << std::endl;
        return;
    }

    settingsFile.close();

    std::cout << "Loaded resolution: " << width << " x " << height << std::endl;

    bool found = false;
    for (size_t i = 0; i < resolutions.size(); ++i) {
        if (resolutions[i].x == width && resolutions[i].y == height) {
            currentResolutionIndex = i;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "No matching resolution found for " << width << "x" << height
            << ". Using default resolution." << std::endl;
        currentResolutionIndex = 0; // Or your desired fallback index
    }
}