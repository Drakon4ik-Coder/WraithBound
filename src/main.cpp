#include "Main.h"
#include "./Entities/Enemies/MeleeMonster.h"
#include "./Entities/Player/Player.h"
#include "EntityManager.h"
#include "../lib_tile_level_loader/LevelSystem.h"
#include "../src/Entities/Entity.h"
#include "../src/SceneManager/SceneManager.h"
#include "../src/Scenes/InGameScene.h"
#include "../src/Scenes/MainMenuScene.h"
#include "../src/Scenes/SettingsScene.h"
#include "../src/Scenes/Scene.h"
#include "../src/Scenes/PauseScene.h"
#include "../src/Scenes/GameOverScene.h"
#include "../src/Scenes/VictoryScene.h"

std::shared_ptr<EntityManager> entityManager;
sf::View camera(sf::Vector2f(400, 300),
                sf::Vector2f(800, 600));
Texture melee_skeleton;
Texture player_sword;
Texture dungeonTexture;

// Globals
std::unique_ptr<sf::RenderWindow> window;
std::unique_ptr<SceneManager> sceneManager;
bool testMode = false;
float testDuration = 10.0f;
const int TILE_SIZE = 32;  
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
            "res/img/Dungeon/dungeon.png");

        LevelSystem::WALL;
        LevelSystem::ENTRANCE;
        LevelSystem::START;
        LevelSystem::EMPTY;

    } catch (const std::exception& e) {
        std::cerr << "Error initializing LevelSystem: " << e.what()
                  << std::endl;
        std::exit(1);
    }
}

void Update(float dt, sf::Clock& timer) {
    // If in test mode, exit after the specified duration
    if (testMode && timer.getElapsedTime().asSeconds() >= testDuration) {
        std::cout << "Exiting test mode after " << testDuration << " seconds." << std::endl;
        window->close();
        return;
    }

    // Handle input and update scenes
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
        else {
            // Reset the view to default if there's no player (e.g., in MainMenu)
            window->setView(window->getDefaultView());
        }
    }
}


void Render() {
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

    sf::Vector2u resolution(800, 600);  // Default resolution
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(resolution.x, resolution.y), "Wraithbound");

    sceneManager = std::make_unique<SceneManager>();

    Load();

    auto settingsScene = std::make_shared<SettingsScene>(sceneManager.get(), *window);
    std::shared_ptr<MainMenuScene> mainMenuScene =
        std::make_shared<MainMenuScene>(sceneManager.get());
    sceneManager->addScene("MainMenu", mainMenuScene);
    std::shared_ptr<PauseScene> pauseScene = std::make_shared<PauseScene>(sceneManager.get());
    sceneManager->addScene("Pause", pauseScene);
    std::shared_ptr<GameOverScene> gameOverScene = std::make_shared<GameOverScene>(sceneManager.get());
    sceneManager->addScene("GameOver", gameOverScene);
    sceneManager->addScene("Settings", settingsScene);
    std::shared_ptr<VictoryScene> victoryScene = std::make_shared<VictoryScene>(sceneManager.get());
    sceneManager->addScene("Victory", victoryScene);
    sceneManager->setActiveScene("MainMenu");  // Start with MainMenu
    mainMenuScene->onActivate();                 

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
