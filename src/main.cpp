#include "Main.h"
#include "EntityManager.h"
#include "./Entities/Enemies/MeleeMonster.h"
#include "./Entities/Player/Player.h"
// Scenes + Manager
#include "../src/SceneManager/SceneManager.h"
#include "../src/Scenes/Scene.h"
#include "../src/Scenes/InGameScene.h"
#include "../src/Scenes/MainMenuScene.h"

std::shared_ptr<EntityManager> entityManager;
std::shared_ptr<Player> player;
sf::View camera(sf::Vector2f(400, 300), sf::Vector2f(800, 600)); // Center and size
Texture melee_skeleton;
Texture playerTexture; // Declare here

// Globals
std::unique_ptr<sf::RenderWindow> window;
std::unique_ptr<SceneManager> sceneManager;
bool testMode = false;
float testDuration = 10.0f; // Duration in seconds for test mode

void Load() {
    if (!melee_skeleton.loadFromFile("res/img/Skeleton_Warrior/Run.png")) {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }
    if (!playerTexture.loadFromFile("res/img/Main Character/Sword_Run/Sword_Run_full.png")) {
        std::cerr << "Failed to load texture: res/img/Player/PlayerTexture.png" << std::endl;
    }
    else {
        std::cout << "Player texture loaded successfully!" << std::endl;
    }
    
    entityManager = std::make_unique<EntityManager>();
    player = std::make_shared<Player>(entityManager.get()); // Pass EntityManager pointer
    player->setTexture(playerTexture); // Pass it to the player
    
    // Add the player and some enemies to the entity manager
    entityManager->AddEntity(player);
    entityManager->AddEntity(std::make_unique<MeleeMonster>(melee_skeleton, sf::Vector2i{ 128, 128 }));
    entityManager->AddEntity(std::make_unique<MeleeMonster>(melee_skeleton, sf::Vector2i{ 256, 256 }));
}

void Update(float dt, sf::Clock& timer) {
    // If in test mode, exit after the specified duration
    if (testMode && timer.getElapsedTime().asSeconds() >= testDuration) {
        std::cout << "Exiting test mode after " << testDuration << " seconds." << std::endl;
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

    // Update all entities via the entity manager
    entityManager->Update(dt);

    // Optionally, update scene-specific logic
    sceneManager->handleInput(*window);
    sceneManager->update(dt);

    // Update the camera to follow the player
    if (player) {
        camera.setCenter(player->getPosition());
        window->setView(camera);
    }
}

void Render() {
    // Render all entities via the entity manager
    entityManager->Render(*window);

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

    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Wraithbound");
    // Initialize SceneManager
    sceneManager = std::make_unique<SceneManager>(); // Proper initialization of sceneManager

    Load();

    // Create and add scenes to the SceneManager
    std::shared_ptr<MainMenuScene> mainMenuScene = std::make_shared<MainMenuScene>();
    std::shared_ptr<InGameScene> inGameScene = std::make_shared<InGameScene>();

    sceneManager->addScene("MainMenu", mainMenuScene);
    sceneManager->addScene("InGame", inGameScene);

    // Set the active scene
    sceneManager->setActiveScene("MainMenu");

    sf::Clock timer; // Timer for test mode
    sf::Clock clock;

    while (window->isOpen()) {
        float dt = clock.restart().asSeconds();

        Update(dt, timer);
        window->clear();
        Render();
        window->display();
    }
    return 0;
}