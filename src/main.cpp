#include "Main.h"
#include "EntityManager.h"
#include "./Entities/Enemies/MeleeMonster.h"
#include "./Entities/Player/Player.h"
// Scenes + Manager
#include "../src/SceneManager/SceneManager.h"
#include "../src/Scenes/Scene.h"
#include "../src/Scenes/InGameScene.h"
#include "../src/Scenes/MainMenuScene.h"

#include <fstream>
using namespace sf;

std::shared_ptr<EntityManager> entityManager;
std::shared_ptr<Player> player;
Texture melee_skeleton;

void Load() {
    if (!melee_skeleton.loadFromFile("res/img/Skeleton_Warrior/Run.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    entityManager = std::make_unique<EntityManager>();
    player = std::make_shared<Player>();

    entityManager->AddEntity(player);

    entityManager->AddEntity(make_unique<MeleeMonster>(melee_skeleton, Vector2i{ 128,128 }));
    entityManager->AddEntity(make_unique<MeleeMonster>(melee_skeleton, Vector2i{ 128,128 }));
}

void Update(RenderWindow& window) {
    static Clock clock;
    float dt = clock.restart().asSeconds();
    Event event;

    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // Update all groups
    entityManager->Update(dt);

    // Optionally, update a specific group
    // entityManager->UpdateGroup("Enemies", dt);
}

void Render(RenderWindow& window) {
    // Render all groups
    entityManager->Render(window);

    // Optionally, render a specific group
    // entityManager->RenderGroup("Players", window);
}


#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

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
    sceneManager.setActiveScene("MainMenu");


    // Optionally set the active scene
    sceneManager.setActiveScene("MainMenu"); // Assuming your SceneManager identifies scenes by names.

    Clock timer; // Timer for test mode
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();

        // If in test mode, exit after the specified duration
        if (testMode && timer.getElapsedTime().asSeconds() >= testDuration) {
            std::cout << "Exiting test mode after " << testDuration << " seconds." << std::endl;
            window.close();
        }
        // Update and render the active scene
        //float dt = clock.restart().asSeconds();
        sceneManager.handleInput(window);
        sceneManager.update(testDuration);
        window.clear();
        sceneManager.render(window);
        window.display();
    }
    return 0;
}