#pragma once
#include "../../lib_tile_level_loader/LevelGenerator.h"
#include "../../lib_tile_level_loader/LevelSystem.h"
#include "../Entities/Player/Player.h"
#include "../EntityManager.h"
#include "Scene.h"

class InGameScene : public Scene {
   private:
    std::shared_ptr<Player> player;
    std::shared_ptr<EntityManager> entityManager;

   public:
    InGameScene(std::shared_ptr<EntityManager> entityManager)
        : entityManager(entityManager) {
        // Create the Player with the entity manager
        player = std::make_shared<Player>(entityManager.get());
        entityManager->AddEntity(player);
    }

    std::shared_ptr<Player> getPlayer() const override { return player; }

    void handleInput(sf::RenderWindow& window) override {
        // Handle input for player and other entities
    }

    void update(float dt) override {
        entityManager->Update(dt);  // Update all entities
    }

    void render(sf::RenderWindow& window) override {
        entityManager->Render(window);  // Render all entities
        LevelSystem::Render(window);
    }

    void spawnMonsters() {
        // Example of spawning a monster
        auto monster =
            std::make_shared<MeleeMonster>(melee_skeleton, sf::Vector2i(68, 68),
                                           player, gameWidth, gameHeight);
        entityManager->AddEntity(monster);
    }

    void onActivate() override {
        // Generate the dungeon level and save it to a file
        generateLevelToFile();

        // Load the generated level into the LevelSystem
        try {
            LevelSystem::loadLevelFile("res/levels/maze.txt", 100.0f);
        } catch (const std::exception& e) {
            std::cerr << "Error loading level: " << e.what() << std::endl;
            return;
        }

        // Find the spawn point and initialize the player
        for (size_t y = 0; y < LevelSystem::getHeight(); ++y) {
            for (size_t x = 0; x < LevelSystem::getWidth(); ++x) {
                if (LevelSystem::getTile({x, y}) == LevelSystem::START) {
                    player->setPosition(LevelSystem::getTilePosition({x, y}));
                    return;
                }
            }
        }

        std::cerr << "Error: No spawn point found in maze!" << std::endl;
    }
};
