// ../WraithBound/src/main.cpp
#include "Main.h"

#include "./Entities/Enemies/MeleeMonster.h"
#include "./Entities/Player/Player.h"
#include "EntityManager.h"
// Scenes + Manager
#include "../lib_tile_level_loader/LevelSystem.h"
#include "../src/Entities/Entity.h"
#include "../src/SceneManager/SceneManager.h"
#include "../src/Scenes/InGameScene.h"
#include "../src/Scenes/MainMenuScene.h"
#include "../src/Scenes/SettingsScene.h"
#include "../src/Scenes/Scene.h"

std::shared_ptr<EntityManager> entityManager;
sf::View camera(sf::Vector2f(400, 300),
                sf::Vector2f(800, 600));  // Center and size
Texture melee_skeleton;
Texture player_sword;
Texture dungeonTexture;

// Globals
std::unique_ptr<sf::RenderWindow> window;
std::unique_ptr<SceneManager> sceneManager;
bool testMode = false;
float testDuration = 10.0f;  // Duration in seconds for test mode
const int TILE_SIZE = 32;    // Change to your tile size
const int TILE_COUNT_X = dungeonTexture.getSize().x / TILE_SIZE;
const int TILE_COUNT_Y = dungeonTexture.getSize().y / TILE_SIZE;

void Load() {
    if (!player_sword.loadFromFile(
            "res/img/Main Character/Sword_Run/Sword_Run_full.png")) {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }
    if (!melee_skeleton.loadFromFile("res/img/Skeleton_Warrior/Run.png")) {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }
    if (!dungeonTexture.loadFromFile("res/img/Dungeon/dungeon.png")) {
        std::cerr << "Failed to load texture!" << std::endl;
    }

    try {
        LevelSystem::loadSpritesheet(
            "res/img/Dungeon/dungeon.png");  // Ensure the spritesheet is loaded

        LevelSystem::WALL;
        LevelSystem::ENTRANCE;
        LevelSystem::START;
        LevelSystem::EMPTY;

    } catch (const std::exception& e) {
        std::cerr << "Error initializing LevelSystem: " << e.what()
                  << std::endl;
        std::exit(1);
    }

    // entityManager = std::make_unique<EntityManager>();
}

void Update(float dt, sf::Clock& timer) {
    // If in test mode, exit after the specified duration
    if (testMode && timer.getElapsedTime().asSeconds() >= testDuration) {
        std::cout << "Exiting test mode after " << testDuration << " seconds."
                  << std::endl;
        window->close();
        return;
    }

    // Poll events
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            return;
        }
    }

    // Update scene-specific logic
    sceneManager->handleInput(*window);
    sceneManager->update(dt);

    // Update the camera to follow the player in the active scene
    auto activeScene = sceneManager->getActiveScene();
    if (activeScene) {
        auto player = activeScene->getPlayer();
        if (player) {
            camera.setCenter(player->getPosition());
            window->setView(camera);
        }
    }
}

void Render() {
    // Optionally, render scene-specific elements
    sceneManager->render(*window);
}

int main(int argc, char* argv[]) {
    // Check for the test mode argument
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--test-mode") {
            testMode = true;
        }
    }
    srand(static_cast<unsigned int>(time(0)));

    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600),
                                                "Wraithbound");
    sceneManager = std::make_unique<SceneManager>();

    Load();

    // Create and add scenes to the SceneManager
    std::shared_ptr<MainMenuScene> mainMenuScene =
        std::make_shared<MainMenuScene>(
            sceneManager.get());  // Pass SceneManager pointer
    // std::shared_ptr<InGameScene> inGameScene =
    // std::make_shared<InGameScene>(entityManager);

    sceneManager->addScene("MainMenu", mainMenuScene);

    std::shared_ptr<SettingsScene> settingsScene = std::make_shared<SettingsScene>(sceneManager.get());
    sceneManager->addScene("Settings", settingsScene);

    // Set the active scene to MainMenu
    sceneManager->setActiveScene("MainMenu");  // Start with MainMenu
    mainMenuScene->onActivate();               // Initialize main menu
    //sceneManager->setActiveScene("Settings");
    

    sf::Clock timer;
    sf::Clock clock;

    while (window->isOpen()) {
        float dt = clock.restart().asSeconds();
        sceneManager->handleInput(*window);
        Update(dt, timer);
        window->clear();
        Render();

        window->display();
    }
    return 0;
}
