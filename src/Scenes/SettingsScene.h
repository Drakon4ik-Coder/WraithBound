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
    SettingsScene(SceneManager* sceneManager, sf::RenderWindow& window);
    void onActivate() override;
    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::RectangleShape fullScreenButton;
    std::vector<sf::Vector2u> resolutions;
    SceneManager* sceneManager;
    size_t currentResolutionIndex;
    sf::RenderWindow& mainWindow;
    bool fullScreen;
    void saveSettings();
    void loadSettings();
    void applyResolution();
    SettingsScene() = delete;
};


SettingsScene::SettingsScene(SceneManager* sceneManager, sf::RenderWindow& window)
    : sceneManager(sceneManager), mainWindow(window) {
    loadSettings();
    resolutions = { {800, 600}, {1024, 768}, {1280, 720}, {1920, 1080}, {sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height} };
    currentResolutionIndex = 0;
    fullScreen = true;
    applyResolution();
}

// Apply the resolution dynamically
void SettingsScene::applyResolution() {
    const sf::Vector2u& resolution = resolutions[currentResolutionIndex];
    // Close the current window and create a new one with the selected resolution
    if(fullScreen){
        mainWindow.create(sf::VideoMode(resolution.x, resolution.y), "WraithBound", sf::Style::Fullscreen);
    } 
    else {
        mainWindow.create(sf::VideoMode(resolution.x, resolution.y), "WraithBound");
    }
    std::cout << "Applied new resolution: " << resolution.x << "x" << resolution.y << std::endl;
}

void SettingsScene::onActivate() {
    loadSettings();
}

void SettingsScene::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
                        static_cast<float>(mousePos.y));

                    if (fullScreenButton.getGlobalBounds().contains(mousePosF)) {
                        fullScreen = !fullScreen;
                    }
                }
        }
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
                // Save settings and apply resolution when Enter is pressed
                saveSettings();
                applyResolution();
            }
            else if (event.key.code == sf::Keyboard::Escape) {
                if (sceneManager) {
                    sceneManager->setActiveScene("MainMenu");
                }
                else {
                    std::cerr << "SceneManager is null!" << std::endl;
                }
            }
        }
    }
}

void SettingsScene::update(float dt) {}

void SettingsScene::render(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");

    sf::Text resolutionText;
    resolutionText.setFont(font);
    resolutionText.setString("Resolution: " + std::to_string(resolutions[currentResolutionIndex].x) + "x" + std::to_string(resolutions[currentResolutionIndex].y));
    resolutionText.setCharacterSize(24);
    resolutionText.setFillColor(sf::Color::White);
    resolutionText.setOrigin(resolutionText.getLocalBounds().width / 2, resolutionText.getLocalBounds().height / 2);
    resolutionText.setPosition(mainWindow.getSize().x/2, 100);

    fullScreenButton.setFillColor(sf::Color::Blue);
    fullScreenButton.setOrigin(fullScreenButton.getLocalBounds().width / 2, fullScreenButton.getLocalBounds().height / 2);
    fullScreenButton.setPosition(mainWindow.getSize().x/2, 150);



    sf::Text fullScreenText;
    fullScreenText.setFont(font);
    if(fullScreen){
        fullScreenText.setString("Fullscreen: On");
    } else {
        fullScreenText.setString("Fullscreen: Off");
    }
    fullScreenText.setCharacterSize(24);
    fullScreenText.setFillColor(sf::Color::White);
    fullScreenText.setOrigin(fullScreenText.getLocalBounds().width / 2, fullScreenText.getLocalBounds().height / 2);
    fullScreenText.setPosition(mainWindow.getSize().x/2, 150);

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setString("Press ESC to return to Main Menu");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setOrigin(exitText.getLocalBounds().width / 2, exitText.getLocalBounds().height / 2);
    exitText.setPosition(mainWindow.getSize().x/2, 200);

    window.draw(resolutionText);
    window.draw(exitText);
    window.draw(fullScreenText);
    window.draw(fullScreenButton);
 
}

void SettingsScene::saveSettings() {

    // Open file
    std::ofstream settingsFile("res/settings/settings.txt");
    if (!settingsFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << "res/settings/settings.txt" << std::endl;
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
    std::ifstream settingsFile("res/settings/settings.txt");

    if (!settingsFile.is_open()) {
        std::cerr << "Failed to open " << "res/settings/settings.txt" << std::endl;
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
        currentResolutionIndex = 0;
    }
}