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

int main() {

    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "WraithBound");

    // Create the SceneManager
    SceneManager sceneManager;

    // Load global assets
    Load();

    // Create and add scenes to the SceneManager
    std::shared_ptr<MainMenuScene> mainMenuScene = std::make_shared<MainMenuScene>();
    std::shared_ptr<InGameScene> inGameScene = std::make_shared<InGameScene>();

    sceneManager.addScene("MainMenu", mainMenuScene);
    sceneManager.addScene("InGame", inGameScene);
    sceneManager.setActiveScene("MainMenu");


    // Optionally set the active scene
    sceneManager.setActiveScene("MainMenu"); // Assuming your SceneManager identifies scenes by names.

    // Main game loop
    sf::Clock clock;
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Update and render the active scene
        float dt = clock.restart().asSeconds();
        sceneManager.handleInput(window);
        sceneManager.update(dt);
        window.clear();
        sceneManager.render(window);
        window.display();
    }

    return 0;
}
