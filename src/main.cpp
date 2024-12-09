#include "Main.h"
#include "EntityManager.h"
#include "./Entities/Enemies/MeleeMonster.h"
#include "./Entities/Player/Player.h"
// Scenes + Manager
#include "../src/SceneManager/SceneManager.h"
#include "../src/Scenes/Scene.h"
#include "../src/Scenes/InGameScene.h"
#include "../src/Scenes/MainMenuScene.h"


//std::shared_ptr<EntityManager> entityManager;
std::shared_ptr<Player> player;
sf::View camera(sf::Vector2f(400, 300), sf::Vector2f(800, 600)); // Center and size
Texture melee_skeleton;

void Load() {
    if (!melee_skeleton.loadFromFile("res/img/Skeleton_Warrior/Run.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    //entityManager = std::make_unique<EntityManager>();
    player = std::make_shared<Player>();
}

int main(int argc, char* argv[]) {
    bool testMode = false;
    float testDuration = 10.0f; // Duration in seconds for test mode

    // Check for the test mode argument
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--test-mode") {
            testMode = true;
        }
    }
    srand(time(0));

    RenderWindow window(VideoMode(gameWidth, gameHeight), "Wraithbound");

    // Create the SceneManager
    SceneManager sceneManager;

    Load();

    // Create and add scenes to the SceneManager
    std::shared_ptr<MainMenuScene> mainMenuScene = std::make_shared<MainMenuScene>();
    std::shared_ptr<InGameScene> inGameScene = std::make_shared<InGameScene>();

    sceneManager.addScene("MainMenu", mainMenuScene);
    sceneManager.addScene("InGame", inGameScene);

    // Optionally set the active scene
    sceneManager.setActiveScene("InGame"); // Assuming your SceneManager identifies scenes by names.

    Clock timer; // Timer for test mode
    while (window.isOpen()) {
        static Clock clock;
        float dt = clock.restart().asSeconds();

        // If in test mode, exit after the specified duration
        if (testMode && timer.getElapsedTime().asSeconds() >= testDuration) {
            std::cout << "Exiting test mode after " << testDuration << " seconds." << std::endl;
            window.close();
        }

        // Update game logic
        sceneManager.handleInput(window);
        sceneManager.update(dt);                 // Update scene logic (includes player)

        // Get the player from the InGameScene and update camera position
        player = std::dynamic_pointer_cast<InGameScene>(sceneManager.getActiveScene())->getPlayer();
        camera.setCenter(player->getPosition());  // Update the camera position
        window.setView(camera);  // Apply the camera

        // Debugging player and camera positions
        // std::cout << "Player position: " << player->getPosition().x << ", " << player->getPosition().y << std::endl;
        // std::cout << "Camera position: " << camera.getCenter().x << ", " << camera.getCenter().y << std::endl;

        // Render game
        window.clear();
        sceneManager.render(window);             // Render all entities in the scene
        window.display();
    }
    return 0;
}