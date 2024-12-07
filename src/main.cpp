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

        sceneManager.handleInput(window);
        sceneManager.update(dt);
        window.clear();
        sceneManager.render(window);
        window.display();
    }
    return 0;
}