#pragma once
#include "../../lib_tile_level_loader/LevelGenerator.h"
#include "../../lib_tile_level_loader/LevelSystem.h"
#include "../Entities/Player/Player.h"
#include "../EntityManager.h"
#include "Scene.h"
#include <SFML/Audio.hpp>

class InGameScene : public Scene {
   private:
    std::shared_ptr<Player> player;
    std::shared_ptr<EntityManager> entityManager;
    sf::Music backgroundMusic;
    SceneManager* sceneManager;

public:
    InGameScene(std::shared_ptr<EntityManager> entityManager, SceneManager* sceneMgr)
        : entityManager(entityManager), sceneManager(sceneMgr) {
        // Create the Player with the entity manager
        player = std::make_shared<Player>(entityManager.get());
        entityManager->AddEntity(player);
    }

    std::shared_ptr<Player> getPlayer() const override { return player; }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (sceneManager) {
                        sceneManager->setActiveScene("Pause");
                    }
                }
            }
        }
    }

    void update(float dt) override {
        entityManager->Update(dt);

        // Check if the player is dead
        if (!player->isAlive()) {
            if (sceneManager) {
                sceneManager->setActiveScene("GameOver");
            }
        }

        checkVictoryCondition();
    }

    void render(sf::RenderWindow& window) override {
        LevelSystem::Render(window);
        entityManager->Render(window);
    }

    void spawnMonsters() {
        // Get spawn points for ENEMY tiles
        auto monsterSpawnPoints = LevelSystem::getMonsterSpawnPoints();

        // Loop through each spawn point and create a monster at that location
        for (const auto& point : monsterSpawnPoints) {
            auto position =
                LevelSystem::getTilePosition(point);  // Get tile position
            auto monster = std::make_shared<MeleeMonster>(
                melee_skeleton,        // Texture
                sf::Vector2i(68, 68),  // Size of the texture frame
                player,        
                position   
            );
            entityManager->AddEntity(monster);
        }
    }

    void onActivate() {
        // Generate the dungeon level and save it to a file
        generateLevelToFile();

        // Load the generated level into the LevelSystem
        try {
            LevelSystem::loadLevelFile("res/levels/maze.txt", 100.0f);
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading level: " << e.what() << std::endl;
            return;
        }

        // Find the spawn point and initialize the player
        for (size_t y = 0; y < LevelSystem::getHeight(); ++y) {
            for (size_t x = 0; x < LevelSystem::getWidth(); ++x) {
                if (LevelSystem::getTile({ x, y }) == LevelSystem::START) {
                    player->setPosition(LevelSystem::getTilePosition({ x, y }));
                    break;
                }
            }
        }

        std::string musicPath = "res/audio/backgorund-music/b-music.mp3";

        // Load and play background music
        if (!backgroundMusic.openFromFile(musicPath)) {
            std::cerr << "Error loading background music!" << std::endl;
        }
        else {
            backgroundMusic.setLoop(true);  
            backgroundMusic.setVolume(50); 
            backgroundMusic.play();
        }
    }

    void checkVictoryCondition() {
        bool hasMonsters = entityManager->hasEntitiesOfType<MeleeMonster>();
        if (!hasMonsters) {
            if (sceneManager) {
                sceneManager->setActiveScene("Victory");
            }
        }
    }

};
